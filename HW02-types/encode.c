#include <stdio.h>
#include <string.h>

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

        unsigned int koi8r[] =
        {       /*ю    а     б     ц     д     е     ф     г     х     и     й     к     л     м     н     о*/
                0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
                /*п    я     р     с     т     у     ж     в     ь     ы     з     ш     э     щ     ч     ъ*/
                0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
                /*Ю    А     Б     Ц     Д     Е     Ф     Г     Х     И     Й     К     Л     М     Н     О*/
                0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
                /*П    Я     Р     С     Т     У     Ж     В     Ь     Ы     З     Ш     Э     Щ     Ч     Ъ*/
                0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
        };

        unsigned int utf8[] = 
        {
                /*ю       а        б       ц       д      е       ф       г        х       и       й       к       л       м       н       о*/
                0xD18E, 0xD0B0, 0xD0B1, 0xD186, 0xD0B4, 0xD0B5, 0xD084, 0xD0B3, 0xD185, 0xD0B8, 0xD0B9, 0xD0BA, 0xD0BB, 0xD0BC, 0xD0BD, 0xD0BE,
                /*п       я        р       с       т      у       ж       в        ь       ы       з       ш       э       щ       ч       ъ*/
                0xD0BF, 0xD18F, 0xD180, 0xD181, 0xD182, 0xD183, 0xD0B6, 0xD0B2, 0xD18C, 0xD18B, 0xD0B7, 0xD188, 0xD18D, 0xD189, 0xD187, 0xD18A,
                /*Ю       А        Б       Ц       Д      Е       Ф       Г        Х       И       Й       К       Л       М       Н       О*/
                0xD0AE, 0xD090, 0xD091, 0xD0A6, 0xD094, 0xD095, 0xD0A4, 0xD093, 0xD0A5, 0xD098, 0xD099, 0xD09A, 0xD09B, 0xD09C, 0xD09D, 0xD09E,
                /*П       Я        Р       С       Т      У       Ж       В        Ь       Ы       З       Ш       Э       Щ       Ч       Ъ*/
                0xD09F, 0xD0AF, 0xD0A0, 0xD0A1, 0xD0A2, 0xD0A3, 0xD096, 0xD092, 0xD0AC, 0xD0AB, 0xD097, 0xD0A8, 0xD0AD, 0xD0A9, 0xD0A7, 0xD0AA
        };
        int c;
        if (strcmp(encoding, "koi8r") == 0) {
                //char d = 0xD0;
                //fputc(d, out);
                while ((c = fgetc(in)) != EOF){
                        if (c < 0x80) {
                                fputc(c, out);
                        } for (int i=0; i<koi8r[i]; i++) {
                                if (c == koi8r[i]) {
                                        unsigned short value = utf8[i];
                                        unsigned char high_byte = (value >> 8);
                                        unsigned char low_byte = value & 0xFF;
                                        fputc(high_byte, out);
                                        fputc(low_byte, out);
                                }
                        }
                }
        }
        else if (strcmp(encoding, "1251") == 0) {
                while ((c = fgetc(in)) != EOF) {
                        // Windows-1251 в Unicode
                        if (c >= 0x80 && c <= 0xFF) {
                        c = c + 0x350;
                        } 
                        // Unicode в UTF-8
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
                        // iso-8859-5 в Unicode
                        if (c >= 0x80 && c <= 0xFF) {
                        c = c + 0x360;
                        } 
                        // Unicode в UTF-8
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