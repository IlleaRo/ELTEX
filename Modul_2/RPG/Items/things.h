//
// Created by illearo on 7/7/23.
//

#ifndef RPG_THINGS_H
#define RPG_THINGS_H

#include <stdbool.h>
#include "stdlib.h"

typedef struct Item{
    char* name;
    int effect;
    short cost;
} Item;

static Item weapons[10];
static Item armors[6];

static Item sacredWeapon[3];
static Item sacredArmor[3];

static Item bosTreasure = {"The seed of Peace", 0, 32767};

void initThings();
bool createKit(Item*,short); // Получить набор от торговца
Item* getRandWeapon();
Item* getRandArmor();

#endif //RPG_THINGS_H
