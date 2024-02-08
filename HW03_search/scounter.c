#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[]) {
    if (argc <= 1){
                printf("Use Syntax: %s filename\n", argv[0]);
                return 1;
        }
    FILE *fd = fopen(argv[1], "rb");
        if(fd == NULL){
                perror("Error opening file");
                return 1;
        }
}