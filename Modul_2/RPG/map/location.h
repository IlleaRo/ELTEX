//
// Created by illearo on 7/8/23.
//

#ifndef RPG_LOCATION_H
#define RPG_LOCATION_H

#define NUMBER_OF_LEVELS 5

// Events
// 0) Enemy
// 1) Trader
// 2) Cave



typedef struct Location{
    char* name;
    int eventType;
} Location;


void initMap(Location*,int);



#endif //RPG_LOCATION_H
