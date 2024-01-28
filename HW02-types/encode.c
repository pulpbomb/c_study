#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>






int main(int argc, char **argv){
        char *p= setlocale(LC_ALL, "rus");
        printf("My LOCALE is %s\n", p);
        if (argc <= 2){
                printf("Use Syntax: %s <input_filename> <encoding> <output_filename>\n", argv[0]);
                return 1;
        }
        FILE *fd = fopen(argv[1], "rb");
        if(fd == NULL){
                perror("Error opening file");
                return 1;
        }
}