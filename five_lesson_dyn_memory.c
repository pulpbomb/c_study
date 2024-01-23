
#include <stdio.h>
#include <stdlib.h>
 
int main(void)
{
#if 0
    // Выделение памяти под массив из 5 элементов с помощью malloc
    int* mas_m = malloc(5 * sizeof(int));
    printf("----malloc--- \n");
    for (size_t i = 0; i < 5; ++i)
    {
        mas_m[i] = i;
        printf("%d ", mas_m[i]);
    }

    // Выделение памяти под массив из 5 элементов с помощь calloc
    int* mas_c = calloc(5, sizeof(int));
    printf("\n----calloc--- \n");
    for (size_t i = 0; i < 5; ++i)
        printf("%d ", mas_c[i]);

    mas_c = realloc(mas_c, 10 * sizeof(int));
    printf("\n----realloc--- \n");
    for (size_t i = 0; i < 10; ++i)
        printf("%d ", mas_c[i]);

    free(mas_m);
    //-------------------------------------------------------------
#endif  
//------------------------------------------------------------------
// двумерные массивы
// вариант 1
#if 0
    int* mas;
    size_t n = 2, m = 3;
    mas = malloc(n * m * sizeof(int));
    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = 0; j < m; ++j)
        {
            printf("mas[%zu][%zu] = ", i, j);
            scanf("%d", (mas + i * m + j));

        }
    }

    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = 0; j < m; ++j)
            printf("%3d", *(mas + i * m + j));
        printf("\n");
    }

    free(mas);
#endif


//вариант 2
#if 0
    int** mas;
    size_t n = 2, m = 3;
    mas = malloc(n * sizeof(int*));
    for (size_t i = 0; i < n; ++i)
    {
        mas[i] = malloc(m * sizeof(int));
        for (size_t j = 0; j < m; ++j)
        {
            printf("mas[%zu][%zu] = ", i, j);
            scanf_s("%d", &mas[i][j]);
        }
    }

    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = 0; j < m; ++j)
            printf("%5d", mas[i][j]);
        printf("\n");
    }

    for (size_t i = 0; i < n; ++i)
        free(mas[i]);
    free(mas);
#endif  

#if 0
// Вариант 3
const int M = 5; // количество строк
const int N = 10; // количество столбцов
int **a = NULL;
int i, j;
 
a = (int**) malloc(M * sizeof(int*) + N * M * sizeof(int));
a[0] = (int*)(a + M);
for (i = 1; i < M; i++) {
    a[i] = a[0] + i * N;
}
for(i = 0; i < M; i++)
    for(j=0; j < N; j++)
        a[i][j] = rand()%31;

for (i = 0; i < M; ++i)
    {
        for (size_t j = 0; j < N; ++j)
            printf("%5d", a[i][j]);
        printf("\n");
    }
 
//Важные действия
free(a);
#endif

// массив со строками разной длины
//вариант 4
#if 0

    int **table;    // указатель для блока памяти для массива указателей
    int *rows;      // указатель для блока памяти для хранения информации по строкам
 
    int rowscount;  // количество строк
    int d;      // вводимое число
     
    printf("Rows count=");
    scanf("%d", &rowscount);// ввод количества строк
 
    // выделяем память для двухмерного массива
    table = calloc(rowscount, sizeof(int*));
    rows = malloc(sizeof(int)*rowscount);
    // цикл по строкам
    for (int i = 0; i<rowscount; i++){
        printf("\nColumns count for row %d=", i);
        scanf("%d", &rows[i]);
        table[i] = calloc(rows[i], sizeof(int)); 
        for (int j = 0; j<rows[i]; j++){
            printf("table[%d][%d]=", i, j);
            scanf("%d", &d);
            table[i][j] = d;
        }
    }
    printf("\n");
     // вывод введенных чисел на консоль
    for (int i = 0; i<rowscount; i++){
        printf("\n");
        for (int j = 0; j<rows[i]; j++){
            printf("%d \t", table[i][j]);
        }
        // освобождение памяти для одной строки
        free(table[i]);
    }     
    // освобождение памяти
    free(table);
    free(rows);
#endif 
    return 0;
}
