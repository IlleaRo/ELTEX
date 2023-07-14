#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "system/StartGame.h"
#include "persons//characters.h"
#include "map//location.h"
#include "system//input.h"


void printRes(bool, const int*, const int*, const int*, const int*, const short*);


int main() {
    int numOfEnemy = 0, earnedMoney = 0, numOfChest = 0, numOfTrader = 0;


    EnemyInit();
    short answer;
    int hpMAX; float luckMAX;
    Item inventory[2];
    srand(time(NULL));
    Location locations[5];
    initThings(); initMap(locations, 5);
    Character hero;
    printf("Input hero name: ");
    fgets(hero.name, 20, stdin);
    printf("\n");
    for (int i = 0; i < 20; ++i)
        if (hero.name[i] == '\n') {
            hero.name[i] ='\0';
            break;
        }
    switch (chooseHero()) {
        case 1: {
            hero.luck = (float) 0.5;
            hero.hp = 200;
            break;
        }
        case 2: {
            hero.luck = (float) 0.7;
            hero.hp = 500;
            break;
        }
        case 3: {
            hero.luck = (float) 1;
            hero.hp = 10000;
            break;
        }
    }

    hpMAX = hero.hp; luckMAX = hero.luck;
    printf("Today hero that name is %s will start him travel to the Raymond pick!\n", hero.name);
    createKit(inventory, 1);
    hero.numPotions = 1;
    hero.weapon = inventory[0];
    hero.armor = inventory[1];
    hero.money = 50;
    printf("Set have been issued to you!\nWeapon: %s (%d)\n Armor: %s (%d)\n Potion: %d\n",
           hero.weapon.name,hero.weapon.effect,hero.armor.name,hero.armor.effect, hero.numPotions);

    bool isLive = true;
    short days;
    for (days = 1; days < 6; ++days) {
        printf("Welcome to %s! Are you wanna skip this location? NO (1) or YES (2)?\n ---> ", locations[days - 1].name);
        while ((answer = readShort()) < 1 || answer > 2) printf("Incorrect answer!\n");
        if (answer == 1) {
            switch (locations[days].eventType) {
                case 0: {
                    numOfEnemy++;
                    if (!fight(&hero, days, &earnedMoney)) {
                        printf("%s is died", hero.name);
                        printRes(false, &earnedMoney, &numOfEnemy, &numOfTrader, &numOfChest, &days);
                        return 0;
                    } else
                        printf("Hero is winner!\n");
                    break;
                }
                case 1: {
                    numOfTrader++;
                    bool isPot = createKit(inventory, days);
                    printf("%s (HP - %d, damage - %d, armor - %d)\n", hero.name, hero.hp, hero.weapon.effect,
                           hero.armor.effect);
                    printf("You met Trader that has fallowing items:\n1) Potion: %s - %d\n2) Weapon: %s (%d) - %d\n"
                           "3) Armor: %s (%d) - %d\n Would you like to buy something (4 to continue)? You have %d $\n",
                           isPot ? "YES" : "NO", 50, inventory[0].name, inventory[0].effect,
                           inventory[0].cost, inventory[1].name, inventory[1].effect, inventory[1].cost, hero.money);
                    while ((answer = readShort()) != 4) {
                        switch (answer) {
                            case 1: {
                                if (hero.money < 50)
                                    printf("You don't have enough moneys!");
                                else {
                                    hero.money -= 50;
                                    hero.numPotions += 1;
                                }
                                break;
                            }
                            case 2: {
                                if (hero.money < inventory[0].cost)
                                    printf("You don't have enough moneys!");
                                else {
                                    hero.money -= inventory[0].cost;
                                }
                                break;
                            }
                            case 3: {
                                if (hero.money < inventory[1].cost)
                                    printf("You don't have enough moneys!");
                                else {
                                    hero.money -= inventory[1].cost;
                                }
                                break;
                            }
                            default:
                                break;
                        }
                        printf("Anything else?\n");
                    }
                    break;
                }
                case 2: {
                    numOfChest++;
                    bool isChest = rand() % 2;
                    printf("Hero notices the cave where there %s!\n",
                           isChest ? "was the chest with treasures!" : " was nothing!");
                    if (!isChest) break;
                    printf("You need choose power of kick power (from 1 to 3)!");
                    while ((answer = readShort()) < 1 || answer > 3) printf("Incorrect answer!\n");
                    if (answer == rand() % 3 + 1) {
                        // 0 - money
                        // 1 - potion
                        // 2 - weapon
                        // 3 - armor
                        switch (rand() % 3) {
                            case 0: {
                                int quantity = rand() % 100;
                                printf("You found money %d!\n", quantity);
                                hero.money += quantity;
                                earnedMoney += quantity;
                                break;
                            }
                            case 1: {
                                printf("You found potion!\n");
                                hero.numPotions++;
                                break;
                            }
                            case 2: {
                                Item *item = getRandWeapon();
                                printf("You successfully opened the chest! You can take %s (%d) - weapon"
                                       "\n YES (1) or NO (2)", item->name, item->effect);
                                while ((answer = readShort()) < 1 || answer > 2) printf("Incorrect answer!\n");
                                if (answer == 1)
                                    hero.weapon = *item;
                                break;
                            }
                            case 3: {
                                Item *item = getRandArmor();
                                printf("You successfully opened the chest! You can take %s (%d) - armor"
                                       "\n YES (1) or NO (2)", item->name, item->effect);
                                while ((answer = readShort()) < 1 || answer > 2) printf("Incorrect answer!\n");
                                if (answer == 1)
                                    hero.armor = *item;
                                break;
                            }
                        }
                    }
                }
            }
        } else {
            hero.hp = hpMAX;
            hero.luck = luckMAX;
        }
    }

    // Встречаем еще раз торговца
    numOfTrader++;
    bool isPot = createKit(inventory, days);
    printf("%s (HP - %d, damage - %d, armor - %d)\n", hero.name, hero.hp, hero.weapon.effect,
           hero.armor.effect);
    printf("You met Trader that has fallowing items:\n1) Potion: %s - %d\n2) Weapon: %s (%d) - %d\n"
           "3) Armor: %s (%d) - %d\n Would you like to buy something (4 to continue)? You have %d $\n",
           isPot ? "YES" : "NO", 50, inventory[0].name, inventory[0].effect,
           inventory[0].cost, inventory[1].name, inventory[1].effect, inventory[1].cost, hero.money);
    while ((answer = readShort()) != 4) {
        switch (answer) {
            case 1: {
                if (hero.money < 50)
                    printf("You don't have enough moneys!");
                else {
                    hero.money -= 50;
                    hero.numPotions += 1;
                }
                break;
            }
            case 2: {
                if (hero.money < inventory[0].cost)
                    printf("You don't have enough moneys!");
                else {
                    hero.money -= inventory[0].cost;
                }
                break;
            }
            case 3: {
                if (hero.money < inventory[1].cost)
                    printf("You don't have enough moneys!");
                else {
                    hero.money -= inventory[1].cost;
                }
                break;
            }
            default:
                break;
        }
        printf("Anything else?\n");
    }
    days++;
    numOfEnemy++;
    if (!fight(&hero, days, &earnedMoney)) {
        printf("%s is died", hero.name);
        printRes(false, &earnedMoney, &numOfEnemy, &numOfTrader, &numOfChest, &days);
        return 0;
    } else
        printf("Hero is winner!\n");
    printRes(true,&earnedMoney,&numOfEnemy, &numOfTrader, &numOfChest, &days);
    return 0;
}

void printRes(const bool isWin,const int* earnedMoney, const int* numOfEnemy, const int* numOfTrader, const int* numOfChest, const short* days){
    printf("\n\n=== RESULTS ===\nYou are %s!\nMoney that were earned:\t%d\nNumber of enemies encountered:\t%d\n"
           "Number of merchants encountered:\t%d\nNumber of discovered chests:\t%d\nTotal days:\t%d\t",
           isWin? "WINNER": "LOOSER",*earnedMoney, *numOfEnemy, *numOfTrader, *numOfChest, *days);
}