//
// Created by illearo on 7/7/23.
//

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "input.h"

char input[INPUT_SIZE];
char* endPtr;

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
