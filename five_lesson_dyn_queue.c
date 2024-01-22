#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

// Пример создания и работы с очередью.
#define QUEUE_SIZE 10

struct ELEMENT
{
    int data;
    struct ELEMENT *next;
};

struct ELEMENT *in_pos=NULL;
struct ELEMENT *out_pos=NULL;
int sizeq = 0;

//добавление нового элемента в очередь
void enqueue(int q)
{
    if(sizeq == 10)
    {
        printf("Queue is full\n");
        return;
    }

    struct ELEMENT *result = (struct ELEMENT *)calloc(1, sizeof(struct ELEMENT));
    result->data = q;
    if (!in_pos)
    {
        in_pos = result;
        out_pos = result;
        sizeq++;
        return;
    }
    in_pos->next = result;
    in_pos = result;
    sizeq++;

}

// выборка элемента из очереди
void dequeue()
{
    if (sizeq == 0)
    {
        printf("Queue is empty\n");
        return;
    }
    struct ELEMENT *p=out_pos;
    out_pos=p->next;
    free(p);
    sizeq--;
}

// вывод очереди
static void displayq()
{
    if (!sizeq)
    {
        printf("Queue is empty\n");
        return;
    }
    struct ELEMENT *p = out_pos;
    while(p)
    {
        printf("%d ",p->data);
        p=p->next;
    }
    printf("\n");
}

int main()
{
    int r;
    for (;;)
    {
        printf("N - add new, D - show, R - remove, Q - quit \n");
        char s[10];
        if (!fgets(s, sizeof(s), stdin))
            break;
        *s = toupper(*s);
        switch (*s)
        {
        case 'N':
            printf("Enter number - ");
            scanf_s("%d",&r);
            fflush(stdin);
            enqueue(r);
            displayq();
            break;
        case 'D':
            displayq();
            break;
        case 'R':
            dequeue();
            displayq();
            break;
        case 'Q':
            return 0;
        }
    }
}
