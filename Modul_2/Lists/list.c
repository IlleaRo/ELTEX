//
// Created by illearo on 7/4/23.
//

#include <stdio.h>
#include <stdlib.h>

#include "list.h"


typedef struct node{ // Узел листа
    struct node* preview;
    struct node* next;
    TYPE* data;
} node;

typedef struct list{
    node* begList;
    int size;
} list;

void pushHead(list** head, TYPE* data){
    if (!*head){
        *head = (list*) malloc(sizeof(list));
        (*head)->size = 0;
    }
    node* tmp = (node*)malloc(sizeof(node));
    tmp->preview = NULL;
    tmp->data = data;
    tmp->next = (*head)->begList;
    if ((*head)->begList != NULL) (*head)->begList->preview = tmp;
    (*head)->begList = tmp;
    (*head)->size++;
} //Вставка в начало

void pushBack(list** head, TYPE* data){
    if (!*head){
        *head = (list*) malloc(sizeof(list));
        (*head)->size = 0;
    }
    node* tmp = (node*)malloc(sizeof(node));
    tmp->data = data;
    tmp->next = NULL;
    if ((*head)->begList == NULL){
        tmp->preview = NULL;
        (*head)->begList = tmp;
        return;
    }
    node* tail = (*head)->begList;
    while (tail->next != NULL) tail = tail->next;
    tmp->preview = tail;
    tail->next = tmp;
    (*head)->size++;
} //Вставка в конец

void pushOnPlace(list** head, int place, TYPE* data)
{
    if (!*head){
        *head = (list*) malloc(sizeof(list));
        (*head)->size = 0;
    }
    if (place == 0) return pushHead(head, data);
    if (place >= (*head)->size) return pushBack(head, data);
    node* tmp = (node*)malloc(sizeof(node));
    tmp->data = data;
    node* tail = (*head)->begList;
    for (int i = 0; i < place - 1; ++i) {
        tail = tail->next;
    }
    tmp->preview = tail;
    tmp->next = tail->next;
    tail->next = tmp;
    (*head)->size++;
} //Вставка на заданную позицию

void printFromHead(list* head)
{
    if (!head || !head->begList) return;
    node* tmp = head->begList;
    printf("Print list from head:\n");
    while (tmp != NULL)
    {
        printf("[%p]: %d\tpreview: [%p]\tnext: [%p]\n", tmp, *(tmp->data), tmp->preview, tmp->next);
        tmp = tmp->next;
    }
    printf("Total %d nodes\n", head->size);
} //Распечатать с начала

void printFromEnd(list* head)
{
    if (!head || !head->begList) return;
    node* tmp = head->begList;
    while (tmp->next != NULL)
        tmp = tmp->next;
    printf("Print list from end:\n");
    do {
        printf("[%p]: %d\tpreview: [%p]\tnext: [%p]\n", tmp, *(tmp->data), tmp->preview, tmp->next);
        tmp = tmp->preview;
    } while (tmp != NULL);
    printf("Total %d nodes\n", head->size);
} // Распечатать с конца

void fillWithRNum(list** head, int size){
    cleanList(*head);
    for (int i = 0; i < size; i++){
        TYPE* data = (TYPE*) malloc(sizeof(TYPE));
        *data = (TYPE) rand() % 10;
        pushHead(head, data);
    }
} //Генерирует лист со случайными элементами

void cleanList(list* head)
{
    if (!head) return;
    if (head->begList) {
        node *tmp = head->begList;
        node *next;
        while (tmp) {
            next = tmp->next;
            free(tmp->data);
            free(tmp);
            tmp = next;
        }
    }
    head->begList = NULL;
    head->size = 0;
} //Очистка листа (полностью)

void editHead(list* head, TYPE* data){
    if (!head || !head->begList) return;
    free(head->begList->data);
    head->begList->data = data;
}
void editEnd(list* head, TYPE* data){
    if (!head || !head->begList) return;
    node* tmp = head->begList;
    while (tmp->next!=NULL)
        tmp = tmp->next;
    free(tmp->data);
    tmp->data = data;
}

void editWithId(list* head, int index,TYPE* data){
    if (!head || !head->begList) return;
    if (index < 1) return editHead(head, data);
    else if (index >= head->size-1) return editEnd(head,data);
    node* tmp = head->begList;
    for (int i = 0; i < index; ++i) tmp = tmp->next;
    free(tmp->data);
    tmp->data = data;
}

void delHead(list* head){
    if (!head || !head->begList) return;
    node* tmp = head->begList;
    head->begList = head->begList->next;
    if (head->begList)
        head->begList->preview = NULL;
    free(tmp->data);
    free(tmp);
    head->size--;
}
void delEnd(list* head){
    if (!head || !head->begList) return;
    node* tmp = head->begList;
    while(tmp->next)
        tmp = tmp->next;
    if (tmp->preview){
        node* newEnd = tmp->preview;
        newEnd->next = NULL;
    } else
        head->begList = NULL;
    free(tmp->data);
    free(tmp);
    head->size--;
}

void delWithIndex(list* head, int index){
    if (!head || !head->begList) return;
    if (index<1) return delHead(head);
    else if (index>=head->size-1) return delEnd(head);
    node* tmp = head->begList;
    for (int i = 0; i < index; ++i)
        tmp = tmp->next;
    tmp->preview->next=tmp->next;
    tmp->next->preview=tmp->preview;
    free(tmp->data);
    free(tmp);
    head->size--;
}

void shakeSort(list* head){
    if (!head || !head->begList) return;
    bool wasChange = false;
    node* tmp = head->begList;
    int* glass;
    node* start = head->begList,*end;
    while (tmp->next!=NULL){
        if (*tmp->data>*tmp->next->data){
            wasChange = true;
            glass = tmp->next->data;
            tmp->next->data=tmp->data;
            tmp->data=glass;
        }
        tmp = tmp->next;
    }
    if (!wasChange) return;
    else wasChange = false;
    end = tmp->preview;
    tmp = end;
    for (int i = 1; i < head->size - 1; i+=2) {
        while (tmp!=start){
            if (*tmp->data<*tmp->preview->data){
                wasChange = true;
                glass = tmp->preview->data;
                tmp->preview->data=tmp->data;
                tmp->data=glass;
            }
            tmp = tmp->preview;
        }
        if (!wasChange) return;
        else wasChange = false;
        start=start->next;
        tmp = start;

        while (tmp!=end){
            if (*tmp->data>*tmp->next->data){
                wasChange = true;
                glass = tmp->next->data;
                tmp->next->data=tmp->data;
                tmp->data=glass;
            }
            tmp = tmp->next;
        }
        if (!wasChange) return;
        else wasChange = false;
        end = tmp->preview;
        tmp = end;
    }
}

void combSort(list* head){
    if (!head || !head->begList) return;
    double factor = 1.2473309;
    int step = head->size-1;
    double formalStep = step;
    node* left = head->begList;
    node* right = left;
    int* glass;
    while (right->next)
        right = right->next;
    while (step >= 1){
        while (true){
            if (*left->data<*right->data){
                glass = left->data;
                left->data=right->data;
                right->data=glass;
            }
            if (right->next){
                left = left->next;
                right = right->next;
            } else
                break;
        }
        formalStep/=factor;
        left = head->begList;
        right = left;
        if (step!=(int) formalStep){
            step = (int) formalStep;
            for (int i = 0; i < step; ++i)
                right = right->next;
        }
    }
}