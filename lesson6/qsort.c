#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <string.h>

int compare_int(const void *a, const void *b) {
    return *(int*)a - *(int*)b;
}

int compare_int_desc(const void *a, const void *b) {
    return compare_int(b, a);
}

int compare_str(const void *a, const void *b) {
    char** pa = (char**)a;
    char** pb = (char**)b;
    return strcmp(*pa, *pb);
}


int main()
{
    setlocale(LC_ALL, "");
    srand(time(NULL));

    int n, i,j;
    clock_t beg, end;
    double time;

    int *a;

    scanf("%d", &n);
    a = (int*) malloc(sizeof(int)*n);

    for(i = 0 ; i < n; i++) {       // input data
        a[i] = rand();
    }

    beg = clock();
    qsort(a, n, sizeof(int), compare_int_desc); // quick sort
    end = clock();
    time = (double)(end - beg) / CLOCKS_PER_SEC;
    printf("\n Time = %f\n", time);

    for(i = 0 ; i < n; i++) {       // output data
        printf("%d ", a[i]);
    }
    /*
    char* mas[] = {"Elena", "Yan", "Alex", "Bob", "Nic"};
    qsort(mas, 5, sizeof(char*), compare_str);
    for(i = 0 ; i < 5; i++) {       // output data
        printf("%s \n", mas[i]);
    }
*/

    return 0;
}
