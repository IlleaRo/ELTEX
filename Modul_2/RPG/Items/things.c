//
// Created by illearo on 7/7/23.
//

#include "things.h"



void init(){
    weapons[0].name = "dagger"; weapons[0].effect = rand() % 20 + 10; weapons[0].cost = rand()%10+10; // Кинжал
    weapons[1].name = "bow"; weapons[1].effect = rand() % 20 + 20; weapons[1].cost = rand()%10+20; // Лук
    weapons[2].name = "spear"; weapons[2].effect = rand() % 20 + 20; weapons[2].cost = rand()%10+30; // Копье
    weapons[3].name = "mace"; weapons[3].effect = rand() % 20 + 30; weapons[3].cost = rand()%10+40; // Молот
    weapons[4].name = "flail"; weapons[4].effect = rand() % 10 + 40; weapons[4].cost = rand()%10+50; // Булова
    weapons[5].name = "sword"; weapons[5].effect = rand() % 30 + 40; weapons[5].cost = rand()%10+60; // Меч
    weapons[6].name = "axe"; weapons[6].effect = rand() % 10 + 40; weapons[6].cost = rand()%10+70; // Топор
    weapons[7].name = "halberd"; weapons[7].effect = rand() % 20 + 50; weapons[7].cost = rand()%10+80; // Копье с клинком
    weapons[8].name = "crossbow"; weapons[8].effect = rand() % 30 + 10; weapons[8].cost = rand()%10+90; // Арбалет
    weapons[9].name = "battleaxe"; weapons[9].effect = rand() % 20 + 60; weapons[9].cost = rand()%10+100; // Боевой топор

    armors[0].name = "burlap clothing"; armors[0].effect = rand() % 10 + 30; armors[0].cost = rand()%10+10; // Одежда из мешковины
    armors[1].name = "armor"; armors[1].effect = rand() % 15 + 40; armors[1].cost = rand()%10+20; // Доспехи
    armors[2].name = "chainmail"; armors[2].effect = rand() % 20 + 50; armors[2].cost = rand()%30+10; // Кольчуга
    armors[3].name = "hauberk"; armors[3].effect = rand() % 15 + 60; armors[3].cost = rand()%40+30; // Латная рубаха
    armors[4].name = "plate armor"; armors[4].effect = rand() % 20 + 70; armors[4].cost = rand()%10+50; // Латы
    armors[5].name = "panoply"; armors[5].effect = rand() % 30 + 40; armors[5].cost = rand()%20+40; // Еще броня

    sacredWeapon[0].name = "Hammer of Devastation"; // Молот Разрушения
    sacredWeapon[0].effect = rand() % 30 + 120; sacredWeapon[0].cost = rand()%20+120;
    sacredWeapon[1].name = "Blade of Immortality"; // Клинок Бессмертия
    sacredWeapon[1].effect = rand() % 30 + 100; sacredWeapon[1].cost = rand()%20+110;
    sacredWeapon[2].name = "Bow of the Archangel"; // Лук Архангела
    sacredWeapon[2].effect = rand() % 30 + 90; sacredWeapon[2].cost = rand()%20+100;

    sacredArmor[0].name = "Armor of Eternal Protection"; // Доспех вечной защиты
    sacredArmor[0].effect = rand() % 20 + 90; sacredArmor[0].cost = rand()%20+100;
    sacredArmor[1].name = "Dragon Scale Armor"; // Драконий панцирь
    sacredArmor[1].effect = rand() % 20 + 100; sacredArmor[1].cost = rand()%20 + 110;
    sacredArmor[2].name = "Shadow Bracers"; // Теневые наручи
    sacredArmor[2].effect = rand() % 20 + 80; sacredArmor[2].cost = rand()%20 + 90;
}

bool createKit(Item* items, short level){
    if (level > 5) {
        items[0] = sacredWeapon[rand()%3];
        items[1] = sacredArmor[rand()%3];
    } else {
        items[0] = weapons[rand()%5+level-1];
        items[1] = armors[rand()%(level)+1];
    }
}

Item* getRandWeapon(){
    return &weapons[rand()%10];
}

Item* getRandArmor(){
    return &weapons[rand()%6];
}