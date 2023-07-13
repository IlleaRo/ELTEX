//
// Created by illearo on 7/7/23.
//

#include <stdio.h>
#include "input.h"


short chooseHero(){
    printf("Choose your hero:\n1) Men (100 health, 50%% luck)\n"
           "2) Azino (1 health, 1000 effect, 50%% luck)\n ---> ");
    short answer;
    while ((answer=readShort())<1 || answer>2){
        printf("Choose your hero:\n1) Men (100 health, 50 effect, 50%% luck)\n"
               "2) Azino (10000 health, 100%% luck)\n ---> ");
    }
    return answer;
}