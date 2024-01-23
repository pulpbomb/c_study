#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

/*
+ 0. Находим eocdr, 0x504b0506, если нашли - это zip, можно работать, если нет, то выводим сообщение, что это не zip-файл.
+ 1. Находим Central directory file header, 0x504b0102, от его начала 29-30 байты - значение длинны имени файла.
+ 2. От начала Central directory file header, 47 байт - это начала имени файла, надо прочитать кол-во байт из переменной длинны файла и отдать в виде строки.
- 3. Цикл, который будет каждый раз смещать указатель на следующий Central directory file header и повторять п.1 и п.2 пока не упремся в EOF или не исчерпаем кол-во total entries.
---- Кол-во total entries = кол-ву файлов в архиве, эта цифра мб кол-вом повторений цикла.
---- Для начала цикла есть:
        a. Смещение поля имени первого файла.
        b. Смещение поля длинны имени первого файла.
        c. Длинна имени первого файла.
        d. Эти значения надо сохранить в переменные внутри цикла. 
        e. В конце итеррации цикла эти смещения надо как-то обновлять...обновятся ли они для сторонних фукнций из которых ранее эти смещения и получили, если внутри цикла эти функции вызывать?
        - вроде бы fread двигает указатель после считывания сам, это где-то само сохраняется или вне функции с fread это стоит запоминать отдельно?
        f. Цикл должен записывать имя переменной (возвращаемый результат из get_filename_from_zip) видимо в динамический массив?
*/

long get_eocdr_offset(char *file_name){
        FILE *fd = fopen(file_name, "rb");
        unsigned char eocdr_sign[] = {0x50, 0x4b, 0x05, 0x06};
        size_t pattern_len = sizeof(eocdr_sign) / sizeof(eocdr_sign[0]);

        unsigned char buffer[pattern_len];
        size_t bytes_read;
        long offset = -1;

        while ((bytes_read = fread(buffer, 1, pattern_len, fd)) == pattern_len) {
                if (memcmp(buffer, eocdr_sign, pattern_len) == 0) {
                offset = ftell(fd) - pattern_len;
                break;
                }
                fseek(fd, -pattern_len + 1, SEEK_CUR);
        }

        if (bytes_read != pattern_len && !feof(fd)) {
                puts("Failed to read file!\n");
                offset = -1;
        }

        fclose(fd);
        return offset;
}

long get_total_enties_offset(char *file_name){
        FILE *fd = fopen(file_name, "rb");
        unsigned char eocdr_sign[] = {0x50, 0x4b, 0x05, 0x06};
        size_t pattern_len = sizeof(eocdr_sign) / sizeof(eocdr_sign[0]);

        unsigned char buffer[pattern_len];
        size_t bytes_read;
        long offset_te = -1;

        while ((bytes_read = fread(buffer, 1, pattern_len, fd)) == pattern_len) {
                if (memcmp(buffer, eocdr_sign, pattern_len) == 0) {
                offset_te = ftell(fd) - pattern_len + 10;
                break;
                }
                fseek(fd, -pattern_len + 1, SEEK_CUR);
        }

        if (bytes_read != pattern_len && !feof(fd)) {
                puts("Failed to read file!\n");
                offset_te = -1;
        }

        fclose(fd);
        return offset_te;
}

unsigned short get_total_enties(char *file_name){
        long offset_te = get_total_enties_offset(file_name);
        if (offset_te == -1) {
                return 1;
        }

        FILE *fd = fopen(file_name, "rb");

        unsigned short te;
        fseek(fd, offset_te, SEEK_SET);
        fread(&te, sizeof(te), 1, fd);
        fclose(fd);
        return te;
}

long get_cdfh_filelen_offset(char *file_name){
        FILE *fd = fopen(file_name, "rb");
        unsigned char cdfh_sign[] = {0x50, 0x4b, 0x01, 0x02};
        size_t pattern_len = sizeof(cdfh_sign) / sizeof(cdfh_sign[0]);
        unsigned char buffer[pattern_len];
        size_t bytes_read;
        long offset_filelen = -1;

        while ((bytes_read = fread(buffer, 1, pattern_len, fd)) == pattern_len) {
                if (memcmp(buffer, cdfh_sign, pattern_len) == 0) {
                offset_filelen = ftell(fd) - pattern_len + 28;
                break;
                }
                fseek(fd, -pattern_len + 1, SEEK_CUR);
        }

        if (bytes_read != pattern_len && !feof(fd)) {
                puts("Failed to read file!\n");
                offset_filelen = -1;
        }

        fclose(fd);
        return offset_filelen;
}

long get_cdfh_filename_offset(char *file_name){
        FILE *fd = fopen(file_name, "rb");
        unsigned char cdfh_sign[] = {0x50, 0x4b, 0x01, 0x02};
        size_t pattern_len = sizeof(cdfh_sign) / sizeof(cdfh_sign[0]);
        unsigned char buffer[pattern_len];
        size_t bytes_read;
        long offset_filename = -1;

        while ((bytes_read = fread(buffer, 1, pattern_len, fd)) == pattern_len) {
                if (memcmp(buffer, cdfh_sign, pattern_len) == 0) {
                offset_filename = ftell(fd) - pattern_len + 46;
                break;
                }
                fseek(fd, -pattern_len + 1, SEEK_CUR);
        }

        if (bytes_read != pattern_len && !feof(fd)) {
                puts("Failed to read file!\n");
                offset_filename = -1;
        }

        fclose(fd);
        return offset_filename;
}

char* get_filename_from_zip(char *file_name){
        long offset_filename = get_cdfh_filename_offset(file_name);
        long offset_filelen = get_cdfh_filelen_offset(file_name);

        if (offset_filename == -1 || offset_filelen == -1) {
                return NULL;
        }

        FILE *fd = fopen(file_name, "rb");

        // Считываем длину имени
        fseek(fd, offset_filelen, SEEK_SET);
        unsigned char name_len[2];
        fread(name_len, 2, 1, fd);
        int name_len_dec = name_len[0] + (name_len[1] << 8);

        // Считываем имя файла
        fseek(fd, offset_filename, SEEK_SET);
        char *zipfilename = (char*)malloc(name_len_dec + 1);
        fread(zipfilename, name_len_dec, 1, fd);
        zipfilename[name_len_dec] = '\0';
        free(zipfilename);
        fclose(fd);
        return zipfilename;
}

int main(int argc, char **argv){
        setlocale(0, "rus");
        if (argc <= 1){
                printf("Use Syntax: %s filename\n", argv[0]);
                return 1;
        }
        //FILE *fd = fopen(argv[1], "rb");
        long offset = get_eocdr_offset(argv[1]);
        long offset_filename = get_cdfh_filename_offset(argv[1]);
        long offset_filelen = get_cdfh_filelen_offset(argv[1]);
        long offset_te = get_total_enties_offset(argv[1]);
        int te = get_total_enties(argv[1]);
        char  *zipfilename = get_filename_from_zip(argv[1]);
        if (offset != -1) {
                printf("EOCDR found, this is ZIP-file.\n");
                if (offset_filelen == -1 || offset_filename == -1) {
                printf("But ZIP-file is empty!\n");
                return 1;
                }
                //debug...
                printf("FILENAME must be at offset %ld\n", offset_filename);
                printf("FILELEN must be at offset %ld\n", offset_filelen);
                printf("Total entries must be at offset %ld\n", offset_te);
                printf("Total entries - %d\n", te);
                printf("Filenames = %s\n", zipfilename);
        }
        else {
                printf("File has no zip signature!");
        }
        //fclose(fd);
        return 0;
}

