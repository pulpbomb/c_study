#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

//int main()
//{
//размер типа
#if 0
    printf("размер char = %zd \n", sizeof(char));
    printf("размер short = %hd \n", sizeof(short));
    printf("размер int = %d \n", sizeof(int));
    printf("размер long = %ld \n", sizeof(long));
    printf("размер long long = %lld \n", sizeof(long long));
    printf("размер float = %d \n", sizeof(float));
    printf("размер double = %zd \n", sizeof(double));
    //printf("������ void = %d \n", sizeof(void));
#endif

#if 0
unsigned int un = 3000000000;   // ������� � 32-������ ����� int
short end = 200;                // � 16-������ ����� short
long big = 65537;
long long veribig = 12345678908642;
printf("un = %u, но не %d\n", un, un); // интерпритирует первый бит в %d как знак минус
printf("end = %hd и %d\n", end, end);
printf("big = %ld, но не %hd\n", big, big);
printf("veribig = %lld, но не %ld\n", veribig, veribig);
#endif


// переполнение
#if 0
    int8_t i8 = 127;
        printf("x = %d \n", i8);
    i8++;
        printf("x + 1 = %d \n", i8);
    i8 *=2;
        printf("x * 2 = %d \n", i8);
#endif

// неявное приведение типов, нельзя выполнять арифметику для разных типов, компилятор приводит их к максимальному типу (usigned int)
#if 0
    int si = -100;
    unsigned int uni = 10U;
    long long int lli = si + uni;
        printf("si + uni = %lld" , lli);
#endif

#if 0
    char ch = 128;
    int i = ch * 10;
        printf("i = %d", i);
#endif

// явное приведение типов
#if 0
    int16_t si_8 =1000;
        printf("si_8 = %d \n", si_8);
        printf("si_8 = %" PRId8, si_8);
#endif



//вещественные числа
#if 0
    float flt = 45000.0;
    double dbl = 5.15e9;
    printf("десятичное представление - %7.1f \n", flt); // всего цифр 7, после запятой 1 (%7.1f)
    printf("float экспоненциальное представление - %e \n", flt);
    printf("float шестнадцатиричное представление - %a \n", flt);
    printf("double %f == %e \n", dbl, dbl);
#endif

//ошибка округления
#if 0
    double a, b;
    a = 2.e10 + 1;
    b = a - 2.e10; // без оптимизации можно получить что-то вроде 1.0000000000000002
    printf("%25.15f", b);
    if (fabs(b) < 0.0000000001)
        printf("\nТочное равенство!");
    else
        printf("\nНеточное равенство!");
#endif

    //return 0;
//}




#pragma pack(push, 1) // отключить выравнивание структуры автоматическое
typedef struct // typedef используется для задания имени уже сущ типу данных
{
  unsigned char x;
  unsigned short y;
  unsigned int z;
} my_arg_t;
#pragma pack(pop) // вернуть выравнивание (рекомендуется всегда возвращать)
int main()
{
printf("sizeof data is %d\n", sizeof(my_arg_t));
my_arg_t my_arg;
  my_arg.z = 0x11223344;
  printf("z = 0x%08X\n", my_arg.z);
  my_arg.y = 0x5566;
  printf("y = 0x%04X\n", my_arg.y);
  my_arg.x = 0x77;
  printf("x = 0x%02X\n", my_arg.x);

    return 0;
}
