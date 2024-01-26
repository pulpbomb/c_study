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

unsigned short get_total_enties(FILE *fd, long offset_te){
        if (offset_te == -1) {
                return 1;
        }
        unsigned short te;
        fseek(fd, offset_te, SEEK_SET);
        fread(&te, sizeof(te), 1, fd);
        return te;
}

long get_cdfh_filelen_offset(FILE *fd, long offset_filelen){
        unsigned char cdfh_sign[] = {0x50, 0x4b, 0x01, 0x02};
        size_t pattern_len = sizeof(cdfh_sign) / sizeof(cdfh_sign[0]);
        unsigned char buffer[pattern_len];
        size_t bytes_read;
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

char* get_filename_from_zip(FILE *fd, long offset_filelen, long offset_filename){
        if (offset_filelen == -1) {
                return NULL;
        }
        fseek(fd, offset_filelen, SEEK_SET);
        unsigned char name_len[2];
        fread(name_len, 2, 1, fd);
        int name_len_dec = name_len[0] + (name_len[1] << 8);

        fseek(fd, offset_filename, SEEK_SET);
        char *zipfilename = (char*)malloc(name_len_dec + 1);
        fread(zipfilename, name_len_dec, 1, fd);
        zipfilename[name_len_dec] = '\0';
        return zipfilename;
        //free(zipfilename);
}

int main(int argc, char **argv){
        char *p= setlocale(LC_ALL, "rus");
        printf("My LOCALE is %s\n", p);
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
        long offset_te = offset + 10;
        int te = get_total_enties(fd, offset_te);
        fseek(fd, 0, SEEK_SET);
        if (offset != -1) {
                printf("EOCDR found, this is ZIP-file.\n");
                printf("Total entries - %d\n", te);
                if (te == 0) {
                        printf("ZIP-file is empty!\n");
                        return 1;
                }
        }
        else {
                printf("File has no zip signature!");
                return 1;
        }
        for(int i=0; i <= te-1; i++){
                        long offset_filelen = get_cdfh_filelen_offset(fd, offset_filelen);
                        long offset_filename = offset_filelen + 18;
                        char  *zipfilename = get_filename_from_zip(fd, offset_filelen, offset_filename);
                        printf("Filenames = %s\n", zipfilename);
                        free(zipfilename);
                        offset_filelen = offset_filename;
        }
        fclose(fd);       
        return 0;
}

