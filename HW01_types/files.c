#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

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

long get_cdfh_filelen_offset(FILE *fd, long offset_filelen){
        fseek(fd, offset_filelen, SEEK_SET);    
        unsigned char cdfh_sign[] = {0x50, 0x4b, 0x01, 0x02};
        size_t pattern_len = sizeof(cdfh_sign) / sizeof(cdfh_sign[0]);
        unsigned char buffer[pattern_len];
        size_t bytes_read;
        //Тут происходит что-то непонятное, на второй итерации цикла в main кусок кода ниже превращается в тыкву...
        while ((bytes_read = fread(buffer, 1, pattern_len, fd)) == pattern_len) {
                if (memcmp(buffer, cdfh_sign, pattern_len) == 0) {
                offset_filelen = ftell(fd) - pattern_len + 28;
                break;
                }
                fseek(fd, -pattern_len + 1, SEEK_CUR);
        }
        if (bytes_read != pattern_len && !feof(fd)) {
                puts("FILE_LEN Failed to read file!\n");
                offset_filelen = -1;
        }
        return offset_filelen;
}

long get_cdfh_filename_offset(FILE *fd, long new_offset_filelen){
        fseek(fd, new_offset_filelen, SEEK_SET);
        long offset_filename = ftell(fd) + 18;
        return offset_filename;
}

char* get_filename_from_zip(FILE *fd, long offset_filelen, long offset_filename){

        if (offset_filelen == -1) {
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
        long offset_filelen = 0;
        if (offset != -1) {
                printf("EOCDR found, this is ZIP-file.\n");
                printf("EOCDR must be at the offset %ld\n", offset);
                printf("Total entries must be at the offset %ld\n", offset_te);
                printf("Total entries - %d\n", te);
                if (offset_filelen == -1) {
                                printf("But ZIP-file is empty!\n");
                                return 1;
                }
                for(int i=0; i != 2; i++){
                        printf("OLD_FILELEN must be at the offset %ld\n", offset_filelen);
                        long new_offset_filelen = get_cdfh_filelen_offset(fd, offset_filelen);
                        printf("\n\n");
                        printf("FILELEN must be at the offset %ld\n", new_offset_filelen);
                        long offset_filename = get_cdfh_filename_offset(fd, new_offset_filelen);
                        printf("FILENAME must be at the offset %ld\n", offset_filename);
                        char  *zipfilename = get_filename_from_zip(fd, new_offset_filelen, offset_filename);
                        printf("Filenames = %s\n", zipfilename);
                        offset_filelen = offset_filename;
                        printf("NEXT_FILELEN must be at the offset %ld\n", offset_filelen);
                        printf("\n\n");
                }
        }
        else {
                printf("File has no zip signature!");
        }
        fclose(fd);       
        return 0;
}

