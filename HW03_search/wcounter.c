#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 1024

typedef struct {
        char* key;
        int value;
} Entry;

Entry table[TABLE_SIZE];

//Берштейн.
unsigned long hash(const char* key) {
        unsigned long hash = 5381;
        int c;
        while ((c = *key++)) {
                hash = (hash * 33 + c) % TABLE_SIZE;
        }
        return hash;
}

void insert(const char* key) {
        unsigned long index = hash(key);
        while (table[index].key != NULL && strcmp(table[index].key, key) != 0) {
                index = (index + 1) % TABLE_SIZE;
        }
        if (table[index].key == NULL) {
                table[index].key = strdup(key);
                table[index].value = 1;
        } else {
                table[index].value++;
        }
}

int main(int argc, char* argv[]) {
        if (argc != 2) {
                printf("Usage: %s <filename>\n", argv[0]);
                return 1;
        }

        FILE* fd = fopen(argv[1], "r");
        if (fd == NULL) {
                printf("Error opening file\n");
                return 1;
        }

        char word[100];
        while (fscanf(fd, "%s", word) == 1) {
                insert(word);
        }

        fclose(fd);

        for (int i = 0; i < TABLE_SIZE; i++) {
                if (table[i].key != NULL) {
                        printf("%s: %d\n", table[i].key, table[i].value);
                }
        }

        return 0;
}