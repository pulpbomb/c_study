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
long get_eocdr_offset(FILE *fd){
        fseek(fd, 0, SEEK_SET);
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
                puts("EOCDR Failed to read file!");
                offset = -1;
        }
        return offset;
}

long get_total_enties_offset(FILE *fd){
        long offset = get_eocdr_offset(fd);
        fseek(fd, 0, SEEK_SET);
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
                puts("TE Failed to read file!\n");
                offset_te = -1;
        }
        return offset_te;
}

unsigned short get_total_enties(FILE *fd){
        long offset_te = get_total_enties_offset(fd);
        if (offset_te == -1) {
                return 1;
        }
        unsigned short te;
        fseek(fd, offset_te, SEEK_SET);
        fread(&te, sizeof(te), 1, fd);
        return te;
}

long get_cdfh_filelen_offset(FILE *fd, ...){
        size_t prev_filelen_offset;
        fseek(fd, 0, SEEK_SET);
        unsigned char cdfh_sign[] = {0x50, 0x4b, 0x01, 0x02};
        size_t pattern_len = sizeof(cdfh_sign) / sizeof(cdfh_sign[0]);
        unsigned char buffer[pattern_len];
        size_t bytes_read;
        long offset_filelen = -1;
        while ((bytes_read = fread(buffer, 1, pattern_len, fd)) == pattern_len) {
                if (memcmp(buffer, cdfh_sign, pattern_len) == 0) {
                offset_filelen = ftell(fd) - pattern_len + 28;
                prev_filelen_offset = offset_filelen;
                break;
                }
                fseek(fd, -pattern_len + 1, SEEK_CUR);
        }
        if (bytes_read != pattern_len && !feof(fd)) {
                puts("FILE_LEN Failed to read file!\n");
                offset_filelen = -1;
        }
        return prev_filelen_offset;
}

long get_cdfh_filename_offset(FILE *fd, ...){
        long prev_filelen_offset = get_cdfh_filelen_offset(fd);
        fseek(fd, prev_filelen_offset, SEEK_SET);
        long offset_filename = ftell(fd) + 18;
        /*
        unsigned char cdfh_sign[] = {0x50, 0x4b, 0x01, 0x02};
        size_t pattern_len = sizeof(cdfh_sign) / sizeof(cdfh_sign[0]);
        unsigned char buffer[pattern_len];
        size_t bytes_read;
        long offset_filename = -1;
        while ((bytes_read = fread(buffer, 1, pattern_len, fd)) == pattern_len) {
                if (memcmp(buffer, cdfh_sign, pattern_len) == 0) {
                offset_filename = ftell(fd) - pattern_len + 18;
                break;
                }
                fseek(fd, -pattern_len + 1, SEEK_CUR);
        }

        if (bytes_read != pattern_len && !feof(fd)) {
                puts("ZIP_FNAME Failed to read file!\n");
                offset_filename = -1;
        }
        */
        return offset_filename;
}

char* get_filename_from_zip(FILE *fd){
        long offset_filename = get_cdfh_filename_offset(fd);
        long offset_filelen = get_cdfh_filelen_offset(fd);

        if (offset_filename == -1 || offset_filelen == -1) {
                return NULL;
        }
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
        FILE *fd = fopen(argv[1], "rb");
        if(fd == NULL){
                perror("Error opening file");
                return 1;
        }
        long offset = get_eocdr_offset(fd);
        long offset_te = get_total_enties_offset(fd);
        int te = get_total_enties(fd);
        long prev_filelen_offset = get_cdfh_filelen_offset(fd);
        long offset_filename = get_cdfh_filename_offset(fd);
        char  *zipfilename = get_filename_from_zip(fd);

        if (offset != -1) {
                printf("EOCDR found, this is ZIP-file.\n");
                printf("EOCDR must be at offset %ld\n", offset);
                if (prev_filelen_offset == -1) {
                printf("But ZIP-file is empty!\n");
                return 1;
                }
                //debug...
                printf("Total entries must be at offset %ld\n", offset_te);
                printf("Total entries - %d\n", te);
                printf("FILELEN must be at offset %ld\n", prev_filelen_offset);
                printf("FILENAME must be at offset %ld\n", offset_filename);
                printf("Filenames = %s\n", zipfilename);
        }
        else {
                printf("File has no zip signature!");
        }
        fclose(fd);       
        return 0;
}

