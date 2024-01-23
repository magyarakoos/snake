#ifndef DEQUE_H
#define DEQUE_H

#include "settings.h"

typedef struct {
    int x;
    Node* next;
} Node;

typedef struct {
    Node* head;
    Node* tail;
    int size;
} Deque;

void init_dq(Deque* dq);
void push_back(Deque* dq, int x);
void push_front(Deque* dq, int x);
void pop_back(Deque* dq);
void pop_front(Deque* dq);

#endif