#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
        if (argc != 4) {
                printf("Usage: %s <input_file> <encoding> <output_file>\n", argv[0]);
                return 1;
        }
    
        const char *input_file = argv[1];
        const char *encoding = argv[2];
        const char *output_file = argv[3];
    
        FILE *in = fopen(input_file, "rb");
        if (in == NULL) {
                printf("Error: could not open input file %s\n", input_file);
                return 1;
        }
    
        FILE *out = fopen(output_file, "wb");
        if (out == NULL) {
                printf("Error: could not open output file %s\n", output_file);
                fclose(in);
                return 1;
        }
    
        char buffer[BUFFER_SIZE];
        size_t bytes_read;
    
        if (strcmp(encoding, "1251") == 0) {

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