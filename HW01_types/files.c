#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

// Идея, разбить файл по 256 байт, помещать их в структуру и каким-то образом искать в них значения...


void main()
{
  FILE * ptrFile = fopen("zipjpeg1.jpg", "rb");
  char c, counter;
  int zip;

if (ptrFile == NULL) perror ("Ошибка открытия файла");
  else { 
        do {
        c = fgetc(ptrFile);
        printf("c = %s", c);
    }
    while (c != EOF); // пока не конец файла
    fclose (ptrFile);
  }
}

/*
int main()
{
  FILE *ptrFile = fopen( "zipjpeg1.jpg" , "rb" );
 
  if (ptrFile == NULL)
  {
      fputs("Ошибка файла", stderr);
      exit(1);
  }
 
  // определяем размер файла
  fseek(ptrFile , 0 , SEEK_END);        // устанавливаем позицию в конец файла
  long lSize = ftell(ptrFile);      // получаем размер в байтах
  printf("filesize: %d bytes \n", lSize);       // выводим размер в байтах
  rewind (ptrFile);     // устанавливаем указатель в конец файла
 
  char *buffer = (char*) malloc(sizeof(char) * lSize);     // выделить память для хранения содержимого файла
  if (buffer == NULL)
  {
      fputs("Ошибка памяти", stderr);
      exit(1);
  }
 
  size_t result = fread(buffer, 1, lSize, ptrFile);     // считываем файл в буфер
  if (result != lSize)
  {
      fputs("Ошибка чтения", stderr);
      exit (1);
  }
 
  //содержимое файла теперь находится в буфере
  puts(buffer);
 
  // завершение работы
  fclose (ptrFile);
  free (buffer);

}
*/