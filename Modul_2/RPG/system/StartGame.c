//
// Created by illearo on 7/7/23.
//

#include <stdio.h>
#include "input.h"


short chooseHero(){
    printf("Choose your hero:\n1) Men (200 health, 50%% luck)\n"
           "2) Talented stranger (500 health, 70%% luck)\n3) Tanos (10000 health, 100%% luck)\n---> ");
    short answer;
    while ((answer=readShort())<1 || answer>3){
        printf("Choose your hero:\n1) Men (200 health, 50%% luck)\n"
               "2) Talented stranger (500 health, 70%% luck)\n3) Tanos (10000 health, 100%% luck)\n---> ");
    }
    return answer;
}