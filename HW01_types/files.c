#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
 
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
      exit(2);
  }
 
  size_t result = fread(buffer, 1, lSize, ptrFile);     // считываем файл в буфер
  if (result != lSize)
  {
      fputs("Ошибка чтения", stderr);
      exit (3);
  }
 
  //содержимое файла теперь находится в буфере
  puts(buffer);
 
  // завершение работы
  fclose (ptrFile);
  free (buffer);
  return 0;
}