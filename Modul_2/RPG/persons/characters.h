//
// Created by illearo on 7/7/23.
//

#ifndef RPG_CHARACTERS_H
#define RPG_CHARACTERS_H

#include "../Items/things.h"


typedef struct Character {
    char name[20];
    int hp;
    float luck;
    int numPotions;
    Item armor;
    Item weapon;
    int money;
} Character;

typedef struct Enemy {
  char* name;
    int hp;
  int damage;
  bool lootIsWeapon;
  Item* loot;
  short cost;
} Enemy;

void EnemyInit(); // Инициализаций врагов
Enemy getEnemy(short level); // Вернуть случайного врага
//void attack(Character* attacker, Character* defender); // Атака персонажа
//void kill(Character* character); // Убийство персонажа
//Enemy* createEnemy(short level); // Инициализация врага

bool fight(Character* hero, short level, int*); // Сражение (выбирается случайный враг)

#endif //RPG_CHARACTERS_H
