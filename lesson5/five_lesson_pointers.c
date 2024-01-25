#include <stdio.h>
#include <stdlib.h>
 
int main(void)
{
    int *x, max;
    int mas[] = {10,65,4,13,7,6,5,9,1,12};
    int *p=mas;
    int *p1 = &mas[1];
    int a=0;
    char *ch = (char *)p1;
    printf("size of int  = %d, size of char = %d\n",sizeof(int), sizeof(char));
    printf("p1 = %p (*p1 = %d),\t p1+1 = %p (*(p1+1) = %d) \n", p1,*p1,p1+1, *(p1+1));
    printf("ch = %p (*ch = %d(%c)), \t ch+1 = %p(*(ch+1) = %d(%c)) \n", ch, *ch,*ch, ch+1,*(ch+1), *(ch+1));
    // Выведем на экран значения массива в шестнадцатиричной форме
    for(int i=0; i < 10; i++){
        x=&mas[i];
        a = *p;
        printf("mas[%d] = %3d (%X)\tx(adress) = %p\tp = %p\t*p = %d\n",i,mas[i],mas[i],x, p,a);
        p++;
    }
    return 0;
}
 
    