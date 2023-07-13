//
// Created by illearo on 7/8/23.
//

#include <stdlib.h>
#include "location.h"

char* locationsName[] = {
"Abandoned Citadel", // Заброшенная цитадель
"Dark Forest", // Темный лес
"Underground Catacombs", // Подземные катакомбы
"Magical Academy", // Магическая академия
"Pirate's Dock", // Причал пиратов
"Lost Temple", // Затерянный храм
"Icy Cave", // Ледяная пещера
"Volcanic Crater", // Вулканический кратер
};

void initMap(Location* locations, int numberOfLevels){
    char newPlace; char* glass;
    for (int i = 0; i < 8; ++i) {
        newPlace = rand() % 8;
        if (i!=newPlace){
            glass = locationsName[newPlace];
            locationsName[newPlace] = locationsName[i];
            locationsName[i] = glass;
        }
    } // Перемешиваем локации
    for (int i = 0; i < numberOfLevels; ++i){
        locations[i].name = locationsName[i];
        locations[i].eventType = rand()%100>40? 0 : rand()%2 + 1;
    }
}
