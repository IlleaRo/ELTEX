//
// Created by illearo on 7/13/23.
//

#include <stdio.h>
#include <stdlib.h>
#include "input.h"

#define INPUT_SIZE 11 // 10 символов максимум!

char input[INPUT_SIZE];
char* endPtr;

int readInt(){
    long value;
    while (true){
        fgets(input,INPUT_SIZE,stdin);
        value = strtol(input,&endPtr,10);
        if (endPtr == input || *endPtr != '\n'){
            printf("Input error! Try again!\n");
        } else break;
    }
    return (int) value;
}