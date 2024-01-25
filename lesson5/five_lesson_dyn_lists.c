#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

struct NODE
{
	void *data;
	struct NODE *next;
//	struct NODE *prev;
};
struct NODE *create_node(int value)
{
	struct NODE *result = (struct NODE *)calloc(1, sizeof(struct NODE));
	result->data = malloc(sizeof(int));
	*(int *)result->data = value;
    printf("%d\n",*(int *)result->data);
	return result;
}

struct NODE *add_first(struct NODE *list, struct NODE *newd)
{
	newd->next = list;
	return newd;
}

struct NODE *add_last(struct NODE* list, struct NODE *newd)
{
    if (!list)
        return newd;

    struct NODE *result = list;
	while (result->next)
		result = result->next;
	result->next = newd;
	return list;
}

void add_after(struct NODE *node, struct NODE *newd)
{
	newd->next = node->next;
	node->next = newd;
}
void process(struct NODE *list, void(*fn)(void *))
{
	struct NODE *node = list;
	while (node)
	{
		fn(node->data);
		node = node->next;
	}
}
struct NODE *find(struct NODE *list, void *data)
{
	struct NODE *node = list;
	while (node)
	{
        if (*(int *)node->data == *(int *)data)
			return node;
		node = node->next;
	}
	return NULL;
}
struct NODE * delete_node(struct NODE *list, struct NODE *element)
{
	struct NODE *result = NULL;
	if (list == element)
	{
		result = element->next;
	}
	else {
		result = list;
		struct NODE *node = list;
		while (node)
		{
			if (node->next == element)
			{
				node->next = element->next;
				break;
			}
            node = node->next;
		}
	}
	free(element);
	return result;
}

void display_list(struct NODE *head)
{
    struct NODE *p = head;
    while(p!=NULL)
    {
        printf("%d ",*(int *)p->data);
        p=p->next;
    }
    printf("\n");
}

// Деревья
struct TREENODE
{
	int data;
	struct TREENODE *left;
	struct TREENODE *right;
};
struct TREENODE *tree_root = NULL;
struct TREENODE *search(struct TREENODE *tree, int x)
{
	if (!tree)
		return NULL;
	if (x == tree->data)
		return tree;
	if (x < tree->data)
		return search(tree->left, x);
	else
		return search(tree->right, x);
}


int main()
{
    int r;
    struct NODE *p;
    struct NODE *list_head = NULL;
	for (;;)
	{
        printf("N - add new, D - display, R - remove, Q - quit\n");
		char s[10];
		if (!fgets(s, sizeof(s), stdin))
			break;
		*s = toupper(*s);
		switch (*s)
		{
        case 'N':
            r = rand()%21;
            p = create_node(r);
            list_head = add_last(list_head,p);
            break;
        case 'D':
            display_list(list_head);
            break;
        case 'R':
            scanf_s("%d",&r);
            fflush(stdin);
            p = find(list_head,&r);
            list_head = delete_node(list_head,p);
            break;
		case 'Q': return 0;
		}
	}
}
