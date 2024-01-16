#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <assert.h>
#include <stdbool.h>

/*
0. Описать структуры заголовков zip-файлов.
1. Прочитать файл.
2. Найти в файле эти структуры.
3. Вывод
*/

/* 
  В Zip-файлах, которые занимали несколько дискет — или томов,
  — EOCDR также содержала информацию о том, какой диск мы сейчас используем,
  на каком диске начинается центральный каталог и т.д.
  Сегодня эта функциональность используется редко
*/

/* Структура End of Central Directory Record */
struct eocdr {
        uint16_t disk_nbr;        /* Поле disk number, содержит окончание Central Directory. */
        uint16_t cd_start_disk;   /* Начало записи Central Directory на диске(файле/потоке). */
        uint16_t disk_cd_entries; /* Кол-во ecdr на диске(файле/потоке). */
        uint16_t cd_entries;      /* Кол-во записей Central Directory на диске(файле/потоке). */
        uint32_t cd_size;         /* Размер Central Directory size in bytes. */
        uint32_t cd_offset;       /* Смещение от начала Central Directory. */
        uint16_t comment_len;     /* Длинна комментария */
        const uint8_t *comment;   /* Сам Комментарий */
};

/* Размер End of Central Directory Record, без комментария. */
#define EOCDR_BASE_SZ 22
#define EOCDR_SIGNATURE 0x06054b50 /* "PK\5\6" little-endian. */
#define EOCDR_POS (file_size - EOCDR_BASE_SZ)

/*Пытаемся найти запись eocdr*/

int64_t getFileSizes(const char *file_name){
	int64_t file_size = 0;
        int64_t noecdr_file_size = 0;
	FILE *fd = fopen(file_name, "rb");
	if(fd == NULL){
		file_size = -1;
	}
	else{
                noecdr_file_size = fseek(fd, -22, SEEK_END);
                noecdr_file_size = ftell(fd);
		fclose(fd);
	}
	return noecdr_file_size;
}

int main(int argc, char **argv){
        if (argc <= 1){
                printf("Use Syntax: %s filename\n", argv[0]);
                return 1;
        }
        char *file_name = argv[1];
	int64_t file_size = getFileSizes(argv[1]);
        int64_t noecdr_file_size = getFileSizes(argv[1]);
	printf("File size: %d\n", noecdr_file_size);
	return 0;
}