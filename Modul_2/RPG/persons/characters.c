//
// Created by illearo on 7/7/23.
//
#include "characters.h"
#include "../system/input.h"
#include <stdlib.h>
#include <stdio.h>


Enemy enemyArray[] = {
        { "Goblin", 50, 10, false, NULL, 50}, // Гоблин
        { "Skeleton", 70, 15, false, NULL, 60}, // Скелет
        { "Wraith", 80, 20, false, NULL, 70}, // Вурдалак
        { "Ogre", 100, 25, false, NULL, 80}, // Огр
        { "Harpy", 60, 30, false, NULL, 90}, // Гарпия
        { "Vampire", 100, 35, false, NULL, 100}, // Вампир
        { "Troll", 120, 40, false, NULL, 200}, // Тролль
        { "Giant", 150, 50, false, NULL, 300}, // Великан
        { "Dragon", 200, 60, false, NULL, 400}, // Дракон
        { "Demon", 210, 70, false, NULL, 500}, // Демон
        {"Mortalox, the World Devourer", 300, 140, false, &bosTreasure, 10000 }
};


void EnemyInit(){
    for (int i = 0; i < 10; ++i)
        if ((enemyArray[i].lootIsWeapon = rand()%2))
            enemyArray[i].loot = &weapons[rand()%10];
        else
            enemyArray[i].loot = &armors[rand()%5];
}


Enemy getEnemy(short level){
    if (level < 6)
        return enemyArray[rand()%5+level];
    return enemyArray[10];
}

void attackEnemy(Character* attacker, Enemy* defender){ // В данной функции герой бьет по врагу
    if (rand()/RAND_MAX >= attacker->luck){ // Результат атаки зависит от удачи
        printf("%s missed\n", attacker->name);
        return;
    } else
        defender->hp -= attacker->weapon.effect; // Герой атакует существо в полном объеме
}

void attackHero(Enemy * attacker, Character * defender){ // В данной функции враг бьет по горою
    if (rand()/RAND_MAX >= 1 - defender->luck){ // Чем выше везение героя, тем больше вероятность промаха по нему
        printf("%s missed\n", attacker->name);
        defender->luck-=(float)0.2;
        if (defender->luck<0) defender->luck = 0;
        return;
    } else
        // Получаемый урон снижается на половину эффективности брони
        defender->hp-= attacker->damage - defender->armor.effect/50;
}


void kill(Character* character){
    printf("%s had died", character->name);
    free(character->name);
}

bool fight(Character* hero, short level, int* moneyStat) {
    Enemy enemy = getEnemy(level); // Получаем случайного врага в зависимости от уровня
    if (level<6){
        if ((enemy.lootIsWeapon = rand()%2))
            enemy.loot = getRandWeapon();
        else
            enemy.loot = getRandArmor();
    }
    printf("You enemy is %s\n",enemy.name);
    short answer;
    while (true) {
        printf("%s is attacking you!\n", enemy.name); // Враг атакует
        attackHero(&enemy,hero);

        // Вывод текущей информации
        printf("%s (HP - %d, damage - %d, LUCK - %0.2f, armor - %d)\t %s (HP - %d, damage - %d)\n",hero->name,
               hero->hp, hero->weapon.effect, hero->luck, hero->armor.effect, enemy.name, enemy.hp, enemy.damage);

        if (hero->hp<1) return false; // Проверяем количество HP героя

        if (hero->numPotions > 0) { // Если у героя есть зелья, то он может его использовать перед атакой/обороной
            printf("Attack (1), defend (2) or drink the potion (3)?\n");
            while ((answer = readShort()) < 1 || answer > 3) printf("Incorrect answer!");
            switch (answer) {
                case 1:{
                    attackEnemy(hero,&enemy); // Атакуем врага
                    hero->luck-= (float) 0.3;
                    if (hero->luck<0) hero->luck = 0;
                    if (enemy.hp<1) { // Проверяем количество здоровья у врага
                        printf("You you have earned $%d!\nWould you like to put %s (%d)?\n YES (1) or NO (2)?",
                               enemy.cost, enemy.loot->name, enemy.loot->effect);
                        hero->money+=enemy.cost;
                        *moneyStat+=enemy.cost;
                        while ((answer = readShort()) < 1 || answer > 2) printf("Incorrect answer!");
                        if (answer == 1){
                            if (enemy.lootIsWeapon)
                                hero->weapon = *enemy.loot;
                            else
                                hero->armor = *enemy.loot;
                        }
                        return true;
                    }
                    break;
                }
                case 2: { // Удача восполняется при обороне
                    hero->luck+=(float) 0.3;
                    if (hero->luck > 1) hero->luck = 1;
                    break;
                }
                case 3: {
                    hero->hp += 30; // Зелье добавляет HP
                    hero->numPotions--;
                    break;
                }
                default:
                    break;
            }

            if (answer == 3) { // Если было выбрано "выпить зелье", то даем игроку еще ход
                printf("%s (HP - %d, damage - %d, LUCK - %0.2f)\t %s (HP - %d, damage - %d)\n",hero->name,
                       hero->hp, hero->weapon.effect, hero->luck, enemy.name, enemy.hp, enemy.damage);
                printf("Attack (1), defend (2)?\n");
                while ((answer = readShort()) < 1 || answer > 2) printf("Incorrect answer!");
                switch (answer) {
                    case 1: {
                        attackEnemy(hero,&enemy);
                        hero->luck-= (float) 0.3;
                        if (hero->luck<0) hero->luck = 0;
                        if (enemy.hp<1) {
                            printf("You you have earned $%d!\nWould you like to put %s (%d)?\n YES (1) or NO (2)?",
                                   enemy.cost, enemy.loot->name, enemy.loot->effect);
                            hero->money+=enemy.cost;
                            *moneyStat+=enemy.cost;
                            while ((answer = readShort()) < 1 || answer > 2) printf("Incorrect answer!");
                            if (answer == 1){
                                if (enemy.lootIsWeapon)
                                    hero->weapon = *enemy.loot;
                                else
                                    hero->armor = *enemy.loot;
                            }
                            return true;
                        }
                        break;
                    }
                    case 2: {
                        hero->luck+=(float) 0.3;
                        if (hero->luck > 1) hero->luck = 1;
                        break;
                    }
                    default:
                        break;
                }
            } else continue;

        } else { // Варианты действий, когда у игрока нет зелья
            printf("%s (HP - %d, damage - %d, LUCK - %0.2f)\t %s (HP - %d, damage - %d)\n",hero->name,
                   hero->hp, hero->weapon.effect, hero->luck, enemy.name, enemy.hp, enemy.damage);
            printf("Attack (1), defend (2)?\n");
            while ((answer = readShort()) < 1 || answer > 2) printf("Incorrect answer!");
            switch (answer) {
                case 1: {
                    attackEnemy(hero,&enemy);
                    hero->luck-= (float) 0.3;
                    if (hero->luck<0) hero->luck = 0;
                    if (enemy.hp<1) {
                        printf("You you have earned $%d!\nWould you like to put %s (%d)?\n YES (1) or NO (2)?",
                               enemy.cost, enemy.loot->name, enemy.loot->effect);
                        hero->money+=enemy.cost;
                        *moneyStat+=enemy.cost;
                        while ((answer = readShort()) < 1 || answer > 2) printf("Incorrect answer!");
                        if (answer == 1){
                            if (enemy.lootIsWeapon)
                                hero->weapon = *enemy.loot;
                            else
                                hero->armor = *enemy.loot;
                        }
                        return true;
                    }
                    break;
                }
                case 2: {
                    hero->luck+=(float) 0.3;
                    if (hero->luck > 1) hero->luck = 1;
                    break;
                }
                default:
                    break;
            }
        }
    }
}