#include "deque.h"

void init_dq(Deque* dq) {
    dq->head = 0;
    dq->tail = 0;
    dq->size = 0;
}

void push_back(Deque* dq, int x) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->x = x;
    newNode->next = 0;

    if (dq->size == 0) {
        dq->size = 1;
        dq->head = newNode;
        dq->tail = newNode;
        return;
    }

    dq->tail->next = newNode;
    dq->tail = dq->tail->next;
}

void push_front(Deque* dq, int x) {

}

void pop_back(Deque* dq) {

}

void pop_front(Deque* dq) {

}
