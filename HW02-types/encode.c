#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
        if (argc != 4) {
                printf("Usage: %s <input_file> <encoding> <output_file>\n", argv[0]);
                return 1;
        }
    
        const char *encoding = argv[2];
    
        FILE *in = fopen(argv[1], "rb");
        if (in == NULL) {
                printf("Error: could not open input file %s\n", argv[1]);
                return 1;
        }
    
        FILE *out = fopen(argv[3], "wb");
        if (out == NULL) {
                printf("Error: could not open output file %s\n", argv[3]);
                fclose(in);
                return 1;
        }
/*
Диапазон номеров символов	Требуемое количество октетов
00000000-0000007F	                1
00000080-000007FF	                2
00000800-0000FFFF	                3
00010000-0010FFFF	                4
Порядок действий примерно такой:
        1. Каждый символ превращаем в Юникод.
        2. Если код символа меньше 128, то к результату добавляем его в неизменном виде.
        3. Если код символа меньше 2048, то берем последние 6 бит (& 0x3F) и первые 5 бит кода ( >> 6) символа.
           К первым 5 битам добавляем 0xC0 и получаем первый байт последовательности, а к последним 6 битам добавляем 0x80 и получаем второй байт.
*/
        int c;
        if (strcmp(encoding, "1251") == 0) {
                while ((c = fgetc(in)) != EOF) {
                        // Windows-1251 to Unicode
                        if (c >= 0x80 && c <= 0xFF) {
                        c = c + 0x350;
                        } 
                        // Unicode to UTF-8
                        if (c < 0x80) {
                                fputc(c, out);
                        } else if (c < 0x800) {
                                fputc(0xC0 | (c >> 6), out);
                                fputc(0x80 | (c & 0x3F), out);
                        } 

                }
        }
        else if (strcmp(encoding, "8859") == 0) {
                while ((c = fgetc(in)) != EOF) {
                        // iso-8859-5 to Unicode
                        if (c >= 0x80 && c <= 0xFF) {
                        c = c + 0x360;
                        } 
                        // Unicode to UTF-8
                        if (c < 0x80) {
                                fputc(c, out);
                        } else if (c < 0x800) {
                                fputc(0xC0 | (c >> 6), out);
                                fputc(0x80 | (c & 0x3F), out);
                        }
                }
        }
        else {
                printf("Error: unsupported input encoding %s\n", encoding);
                fclose(in);
                fclose(out);
                return 1;
        }
    
        fclose(in);
        fclose(out);
        return 0;
}