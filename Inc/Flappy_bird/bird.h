#ifndef BIRD_H
#define BIRD_H


#include<stdint.h>
#include "floor.h"


#define BIRD_INITIAL_POS_X 5
#define BIRD_INITIAL_POS_Y 15
#define BIRD_BITMAP_WIDTH 16
#define BIRD_BITMAP_HEIGHT 8


typedef struct Bird{

    SceneType type;

    uint8_t x;
    uint8_t y;
    uint8_t w;
    uint8_t h;

    uint8_t *bitmap;


    void (*render)(struct Bird *this);
    void (*position_update)(struct Bird *this,uint8_t input);
    uint8_t (*detect_collision)(struct Bird *this,struct Floor *floor);
} Bird;

void bird_init(Bird *this);





#endif