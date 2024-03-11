#include <stdio.h>
#include <stdlib.h>

struct Node {
    long data;
    struct Node* next;
};

void print_int(long x) {
    printf("%ld ", x);
}

int p(long x) {
    return x & 1;
}

struct Node* add_element(long x, struct Node* next) {
    struct Node* node = malloc(sizeof(struct Node));
    if (node == NULL) {
        abort();
    }
    node->data = x;
    node->next = next;
    return node;
}

void m(struct Node* node, void (*func)(long)) {
    if (node == NULL) {
        return;
    }
    func(node->data);
    m(node->next, func);
}

struct Node* f(struct Node* node, long x, long y, int (*func)(long)) {
    if (node == NULL) {
        return NULL;
    }
    struct Node* result = f(node->next, x, y, func);
    if (func(node->data)) {
        return add_element(node->data, result);
    } else {
        return result;
    }
}

int main() {
    long data[] = {4, 8, 15, 16, 23, 42};
    int data_length = sizeof(data) / sizeof(data[0]);
    struct Node* head = NULL;
    
    for (int i = data_length - 1; i >= 0; i--) {
        head = add_element(data[i], head);
    }
    
    m(head, &print_int);
    printf("\n");
    
    head = f(head, 0, 0, &p);
    m(head, &print_int);
    printf("\n");
    
    return 0;
}