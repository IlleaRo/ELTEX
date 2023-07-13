#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "list.h"

#define INPUT_SIZE 10

char input[INPUT_SIZE];
char* endPtr;
short act[] = {0,0,0};

short readShort(); // Прочитать short
int readInt(); // Прочитать int
void selectAction(); // Меню

int main(){
    srand(time(NULL));
    list* head = NULL;
    while (true){
        printf("Press Enter!");
        getchar();
        system("clear");
        selectAction();
        switch (act[0]) {
            case 1: {
                TYPE* data = (TYPE*) malloc(sizeof(TYPE));
                printf("Input value ---> ");
                *data = readInt();
                switch (act[1]) {
                    case 1: {
                        pushHead(&head, data);
                        break;
                    }
                    case 2: {
                        pushBack(&head, data);
                        break;
                    }
                    case 3: {
                        pushOnPlace(&head,act[2], data);
                        break;
                    }
                }
                break;
            }
            case 2: {
                switch (act[1]) {
                    case 1: {
                        delHead(head);
                        break;
                    }
                    case 2: {
                        delEnd(head);
                        break;
                    }
                    case 3: {
                        delWithIndex(head,act[2]);
                        break;
                    }
                }
                break;
            }
            case 3: {
                TYPE* data = (TYPE*) malloc(sizeof(TYPE));
                printf("Input value ---> ");
                *data = readInt();
                switch (act[1]) {
                    case 1: {
                        editHead(head, data);
                        break;
                    }
                    case 2: {
                        editEnd(head, data);
                        break;
                    }
                    case 3: {
                        editWithId(head,act[2], data);
                        break;
                    }
                }
                break;
            }
            case 4: {
                switch (act[1]) {
                    case 1:{
                        printFromHead(head);
                        break;
                    }
                    case 2:
                        printFromEnd(head);
                }
                break;
            }
            case 5:{
                printf("How many elements do you need?\n---> ");
                int size;
                while ((size = readInt()) == 0) printf("Incorrect amount!\n ---> ");
                fillWithRNum(&head,size);
                break;
            }
            case 6:{
                printf("Shaker sort:\n");
                shakeSort(head);
                printFromHead(head);
                printf("Comb sort:\n");
                combSort(head);
                printFromHead(head);
                break;
            }
            case 7:{
                cleanList(head);
                free(head);
                return 0;
            }
            default:
                break;
        }
    }
}


short readShort(){
    long int value;
    while (true){
        fgets(input,INPUT_SIZE,stdin);
        value = strtol(input,&endPtr,10);
        if (endPtr == input || *endPtr != '\n'){
            printf("Input error! Try again!\n");
        } else break;
    }
    return (short) value;
}

int readInt(){
    long int value;
    while (true){
        fgets(input,INPUT_SIZE,stdin);
        value = strtol(input,&endPtr,10);
        if (endPtr == input || *endPtr != '\n'){
            printf("Input error! Try again!\n");
        } else break;
    }
    return (int) value;
}

// Распечатать меню
void selectAction(){
    for (int i = 0; i < 3; ++i)
        act[i]=0;
    printf("1) Insert new node\n2) Delete existing node\n3) Edite existing node\n4) Print list\n"
           "5) Fill list with random values\n6) Additional functions\n7) Exit\n---> ");
    goto input;
    input:
    while ((act[0] = readShort()) < 1) printf("Incorrect answer!\n");
    switch (act[0]) {
        case 1:
        case 2:
        case 3:{
            printf("From head (1) or end (2), with index(3)?\n---> ");
            while ((act[1] = readShort()) < 1 || act[1] > 3) printf("Incorrect answer!\n ---> ");
            if (act[1]==3){
                printf("Input index ---> ");
                while ((act[2] = readShort()) < 0) printf("Incorrect answer!\n ---> ");
                printf("\n");
            }
            break;
        }
        case 4:{
            printf("From head (1) or end (2) ---> ");
            while ((act[1] = readShort()) < 1 || act[1] > 2) printf("Incorrect answer!\n ---> ");
            printf("\n");
            break;
        }
        case 5:
        case 6:
        case 7:
            break;
        default:{
            printf("Incorrect answer!\n ---> ");
            goto input;
        }
    }
}