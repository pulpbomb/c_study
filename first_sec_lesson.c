//TEST_TEST_TEST

#include <stdio.h>
#include <stdlib.h>

/*
int main() {
  int number = 11;
  int count  = number / 4;
  int left   = number % 4;
  int i = 0;
  
  while (left-- > 0) {
    printf("letf_i = %d \n", i++);
  }
  while (count-- > 0) {
    printf("count1 = %d \n", i++);
    printf("count2 = %d \n", i++);
    printf("count3 = %d \n", i++);
    printf("count4 = %d \n", i++);
  }  
  
  printf("%d", i);
}
*/


/*
int main() {
    int a = 0;
    int x = a++;
    //int y = ++x;
    printf(" a = %d ", a);
}
*/

// auto - static
/*
int main()
{
    for (int i = 0; i <= 6; ++i)
        printf("%d\n", i);
}
*/

/* 
int main()
{
   // printf("c:\today\new\begin.txt ");

    int o = 020, h = 0x20, d = 20, b = 0b111;
    printf("dec = %d oct = %o hex = %x", d, d, d);


   // printf("7 / 2 = %d \n", 7 / 2);
   // printf("7 / 2 = %.3f \n", (float)7/2);
   // printf("7 / 2 = %.2f \n", 7/2 + 0.);

}
*/
//  ++  --
/*
    int x = 5, y,z;

    y = ++x;
    z = x++;
    printf("y = %d \n", y);
    printf("z = %d \n", z);
    printf("x = %d \n", x);
*/

  //  int x = 3;                          //    11 -> 1 = 01    11 <- 1 = 110
 //   printf(" x >> 1 = %d \n x << 1 = %d \n  x = %d ", x>>1, x<<1, x);

   // printf("%d", ~(x - 1));

  /*
    int b = 0b00000001;
    if (x & b)
        printf("%d is odd", x);
    else
        printf("%d is even", x);
*/

//}


/*#define sum_x_y(x, y) x + y
#define max(x, y) (x > y ? x : y)
#define autoinc(x, y)  \
    (x)++;             \
    (y)++;

int main(){
    int a = 5, b = 10, rez;
  //  rez = sum_x_y(a,b)* 2;
  //      printf("%d", rez);

  //  rez = max(a, b++);
  //      printf(" rez = %d \n b = %d", rez, b);

    if (a > b)
        autoinc(a, b);

    printf("\n a = %d \n b = %d", a, b);
}
*/

/*#define sum(max, out) {             \
    int total = 0;                  \
    for(int i = 0; i <= max; i++)   \
       total += i;                  \
     out = total;                   \
        }

int main() {
    int out, total = 100;
    sum(5, out);
    printf("out = %d total = %d", out, total);

}
*/


/*
void f_add(int *x, int *y){
    *x = *x + 1;
    *y = *y - 1;
}

int main()
{
    int x1 = 20, y1 = 5;
    printf("Address x = %p", &x1);

    f_add(&x1, &y1);
    printf(" x1 = %d \n y1 = %d", x1, y1);


    return 0;
}
*/

/*
void set_el(int *a, int const *b){
    a[0] = 3;
}

int main(){
    int a[10];
    int const *b = a;
    for(int i = 0; i < 10; i++)
        printf("%d ", a[i]);
    printf("\n");
    set_el(a, b);

    for(int i = 0; i < 10; i++)
        printf("%d ", b[i] );

}
*/

int main(){
    int x = 10;
    int *p;
    p = &x;
    int y = *p;
    printf("память = %x\nзначение = %d", p, y);
    return 0;
}