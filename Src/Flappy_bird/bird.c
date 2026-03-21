

#include "sh1106_graphics.h"
#include "game.h"
extern const uint8_t bird1[];
extern const uint8_t bird2[];

void bird_render(Bird *this){
    static uint8_t index = 0;

    if(index < 2)
        this->bitmap = (uint8_t*)bird1;
    else
        this->bitmap = (uint8_t*)bird2;

    index = (index + 1) % 4;

    gfx_draw_bitmap(this->x,this->y,this->bitmap,this->w,this->h);
}

void bird_position_update(Bird *this,uint8_t input){
    if(input)
        this->x -= 1;
    else
        this->x += 1;
}

uint8_t bird_detect_collision(struct Bird *this,struct Floor *floor){
    uint8_t bx0 = this->x;
    // uint8_t by0 = this->y;
    uint8_t bx1 = this->x + this->h - 1;
    // uint8_t by1 = this->y + this->w - 1;
    // four co-ordinate of floor (fx0,fy0) (fx1,fy0) (fx1,fy1) (fx0,fy1)
    uint8_t fx0 = floor->x;
    // uint8_t fy0 = floor->y;
    uint8_t fx1 = floor->x + floor->h - 1;
    // uint8_t fy1 = floor->y + floor->w - 1;
    

    if(fx1 < bx0 || fx0 > bx1)
        return 0;
    return 1;

}

static void single_bird_init(Bird *this){
    this->x = BIRD_INITIAL_POS_X;
    this->y = BIRD_INITIAL_POS_Y;
    this->w = BIRD_BITMAP_WIDTH;
    this->h = BIRD_BITMAP_HEIGHT;
}

void bird_init(Bird *this){

    this->render = bird_render;
    this->position_update = bird_position_update;
    this->detect_collision = bird_detect_collision;

    this->type = BIRD;
    
    this->init = single_bird_init;
}

