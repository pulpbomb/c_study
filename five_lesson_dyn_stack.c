#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale.h>

// Пример создания и работы со стеком.

#define STACK_SIZE 10

struct ELEMENT
{
    int data;
    struct ELEMENT *next;
    struct ELEMENT *prev;
};

struct ELEMENT *in_out_pos=NULL;
struct ELEMENT *head=NULL;
int sizeq = 0;

//добавление нового элемента в стек
void push(int q)
{
    if(sizeq == 10)
    {
        printf("Stack is full\n");
        return;
    }

    struct ELEMENT *result = (struct ELEMENT *)calloc(1, sizeof(struct ELEMENT));
    result->data = q;
    if (!in_out_pos)
    {
        in_out_pos = result;
        head = result;
        sizeq++;
        return;
    }
    result->prev =  in_out_pos;
    in_out_pos->next = result;
    in_out_pos = result;
    sizeq++;

}

// выборка элемента из стека
void pop()
{
    if (sizeq == 0)
    {
        printf("Stack is empty\n");
        return;
    }
    struct ELEMENT *p=in_out_pos->prev;
    p->next = NULL;
    free(in_out_pos);
    in_out_pos = p;
    sizeq--;
}

// вывод очереди
static void displays()
{
    if (!sizeq)
    {
        printf("Queue is empty\n");
        return;
    }
    struct ELEMENT *p = head;
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
        printf("N - push new, D - show, P - pop, Q - quit \n");
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
            push(r);
            displays();
            break;
        case 'D':
            displays();
            break;
        case 'P':
            pop();
            displays();
            break;
        case 'Q':
            return 0;
        }
    }
}
