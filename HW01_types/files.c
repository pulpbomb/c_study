#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
//#include <stdbool.h>

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

/* Читаем 16 и 32 бит в little-endian and смещаем p вперед после этого. ( скорее всего потому что у нас минимаьная )*/

uint16_t read16le(const void *p)
{
    return 0;
}

uint32_t read32le(const void *p)
{
    return 0;
}

#define READ16(p) ((p) += 2, read16le((p) - 2))
#define READ32(p) ((p) += 4, read32le((p) - 4))

/* Размер End of Central Directory Record, без комментария. */
#define EOCDR_BASE_SZ 22
#define EOCDR_SIGNATURE 0x06054b50  /* "PK\5\6" little-endian. */

#define FILE_PATH "zipjpeg1.jpg"

/*Читаем файл, узнаем его размер*/
int64_t getFileSize(const char* file_name){
	int64_t file_size = 0;
	FILE* fd = fopen(file_name, "rb");
	if(fd == NULL){
		file_size = -1;
	}
	else{
		fseek(fd, 0, SEEK_END);
		file_size = ftello(fd);
		fclose(fd);
	}
	return file_size;
}

int main(int argc, char **argv){
        if (argc <= 1){
                printf("Use Syntax: %s filename\n", argv[0]);
                return 1;
        }
        size_t size = sizeof(struct eocdr);
        printf("Size of struct eocdr = %u\n", size);
        char *file_name = argv[1];
	int64_t file_size = getFileSize(argv[1]);
	printf("File size: %lld\n", file_size);
	return 0;
}