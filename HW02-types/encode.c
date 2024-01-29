#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

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
    
        char buffer[BUFFER_SIZE];
        size_t bytes_read;
        int c;
        if (strcmp(encoding, "1251") == 0) {
                while ((c = fgetc(in)) != EOF) {
                        if (c >= 0x80 && c <= 0xFF) {
                        // Windows-1251 to Unicode
                        c = c + 0x350;
                        } 
                        // Unicode to UTF-8
                        if (c < 0x80) {
                                fputc(c, out);
                        } else if (c < 0x800) {
                                fputc(0xC0 | (c >> 6), out);
                                fputc(0x80 | (c & 0x3F), out);
                                } else {
                                fputc(0xE0 | (c >> 12), out);
                                fputc(0x80 | ((c >> 6) & 0x3F), out);
                                fputc(0x80 | (c & 0x3F), out);
                                }
                }
        } else {
                printf("Error: unsupported input encoding %s\n", encoding);
                fclose(in);
                fclose(out);
                return 1;
        }
    
        fclose(in);
        fclose(out);
        return 0;
}