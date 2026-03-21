
#include "sh1106_graphics.h"
#include "game.h"


void floor_render(Floor *this){
    gfx_draw_line(this->x,this->y, (this->x + this->h - 1),(this->y + this->w - 1),1);
}

static void single_floor_init(Floor *this,uint8_t is_up){

    if(is_up){
        
        this->x = UP_FLOOR_INITIAL_POS_X;
        this->y = UP_FLOOR_INITIAL_POS_Y;
        this->w = UP_FLOOR_WIDTH;
        this->h = UP_FLOOR_HEIGHT;
        
    }else{

        this->x = DOWN_FLOOR_INITIAL_POS_X;
        this->y = DOWN_FLOOR_INITIAL_POS_Y;
        this->w = DOWN_FLOOR_WIDTH;
        this->h = DOWN_FLOOR_HEIGHT;
    
    }

} 

void floor_init(Floor *this){

    this->render = floor_render;
    
    this->type = FLOOR;

    this->init = single_floor_init;
     
}


