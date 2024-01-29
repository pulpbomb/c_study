#include <stdio.h>
#include <stdlib.h>
#include <time.h>
typedef  int Item;
#define less(A, B) ((A) < (B))
#define exch(A, B) {Item t = A; A = B; B = t;}
#define compexch(A, B) if (less(B, A)) exch(A, B)
#define N 10

void print_sort(Item a[], int beg, int end, int indx, int indx2)
{
    int i;
    printf("Step %3d: ", indx);
    for (i = beg; i <= end; i++)
        if (indx2){
            if (i == indx || i == indx2)
                printf("%4d*", a[i]);
            else
                printf("%4d ", a[i]);
        }
        else
            printf("%4d ", a[i]);
    printf("\n");
}

// сортировка вставками
void sort(Item a[], int beg, int end)
{
    int i, j;
    for (i = beg + 1; i <= end; i++){
        for (j = i; j > beg; j--)
            compexch(a[j-1], a[j]);
        print_sort(a, beg, end, i, 0);
    }
}

// Улучшенная сортировка вставками
void sort_insert(Item a[], int beg, int end)
{
    int i;
    // сначала находим минимальный, переносим его в нулевую ячейку
    for (i = end; i > beg; i--)
        compexch(a[i-1] , a[i]);
    print_sort(a, beg, end, 1, 0);

    // рассматриваем оставшийся массив, для каждого элемента находим его место, остальные сдвигаем
    for (i = beg+2; i <= end; i++) {
        int j = i;
        Item v = a[i];
        while (less(v, a[j-1])) {
            a[j] = a[j-1];
            j--;
        }
        a[j] = v;
        print_sort(a, beg, end, i, j);
    }
}

// Сортировка выбором
void sort_select(Item a[], int beg, int end)  // selection sort
{
    int i, j;
    for (i = beg; i < end; i++) {
        int min = i;
        for (j = i + 1; j <= end; j++)
            if (less(a[j], a[min]))
                min = j;
        exch(a[i], a[min]);
        print_sort(a, beg, end, i, min);
    }
}

// пузырьковая сортировка
void sort_bubble(Item a[], int beg, int end)
{
    int i, j;
    for (i = beg; i < end; i++){
        for (j = end; j > i; j --)
            compexch(a[j-1], a[j]);
        print_sort(a, beg, end, i, 0);
    }
}

// шейкерная сортировка
void sort_shaker(Item a[], int beg, int end)
{
    int i, b = beg, e = end, indx = 1;
    while (beg <= end){
        for (i = beg; i < end; i++)
            compexch(a[i], a[i+1]);
        end--;

        printf("Step %3d: ", indx);
        for (i = b; i <= e; i++)
            printf("%4d ", a[i]);
        printf("\n");
        indx++;

        for (i = end; i > beg; i--)
            compexch(a[i-1], a[i]);
        beg++;

        printf("Step %3d: ", indx);
        for (i = b; i <= e; i++)
            printf("%4d ", a[i]);
        printf("\n");
        indx++;
    }
}

// двоичный поиск
void btree_found(Item a[], int beg, int end, int num)
{
    int flag = 0, middle;
    while (flag == 0 && beg <= end){
        middle = (beg + end) / 2;   /// !!!!!!!!!!!!!!
        if (num == a[middle]){
            flag = 1;
            printf("\n Found in %d", middle);
        }
        else {
            if (num < a[middle])
                end = middle - 1;
            if (num > a[middle])
                beg = middle + 1;
        }
    }
    if (flag == 0)
        printf("\n Not found");
}


int main()
{
    int i, num_sort, num;
    Item *a = malloc(N * sizeof(Item));

    printf(" 1 - insert sort \n 2 - modify insert sort \n 3 - select sort \n 4 - bubble sort \n 5 - shaker sort \n 0 - Exit \n Input: ");
    scanf("%d", &num_sort);

    while (num_sort) {
        srand(time(NULL));
        printf("          ");
        for (i = 0; i < N; i++) {
            a[i] = rand() % 100;
            printf("%4d ", a[i]);
        }
        printf("\n");

        switch (num_sort){
        case 1:
            sort(a, 0, N-1);
            break;
        case 2:
            sort_insert(a, 0, N-1);
            break;
        case 3:
            sort_select(a, 0, N-1);
            break;
        case 4:
            sort_bubble(a, 0, N-1);
            break;
        case 5:
            sort_shaker(a, 0, N-1);
            break;
        }
        printf("\nRezult:   ");
        for (i = 0; i < N; i++)
            printf("%4d ", a[i]);
        printf("\n");

        printf(" 1 - insert sort \n 2 - modify insert sort \n 3 - select sort \n 4 - bubble sort \n 5 - shaker sort 0 - Exit \n Input: ");
        scanf("%d", &num_sort);
    }


    printf("Input number: ");
    scanf("%d", &num);
    btree_found(a, 0, N-1, num);


    return 0;
}

