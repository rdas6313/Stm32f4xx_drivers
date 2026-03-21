
#include "game.h"
#include <stdlib.h>


static uint8_t current_x;
static uint8_t current_y;


void pipe_render(Pipe *this){
    gfx_draw_fill_rect(this->x, this->y, this->w, this->h,1);
}

uint8_t pipe_detect_collision(Pipe *this,Bird *bird){
    uint8_t px0 = this->x;
    uint8_t py0 = this->y;
    uint8_t px1 = this->x + this->h - 1;
    uint8_t py1 = this->y + this->w - 1;
    // four co-ordinate (px0,py0) (px1,py0) (px1,py1) (px0,py1)
    uint8_t bx0 = bird->x;
    uint8_t by0 = bird->y;
    uint8_t bx1 = bird->x + bird->h - 1;
    uint8_t by1 = bird->y + bird->w - 1;

    
    if(px0 > bx1 || px1 < bx0 || py1 < by0 || by1 < py0)
        return 0;

    return 1;

}

void pipe_position_update(Pipe *this){
    this->y -= 1;
    if(this->y == 255){
        current_x = (current_x + 1) % 2;
        this->y = SH1106_WIDTH + MIN_PIPE_WIDTH_GAP;
        this->h = rand() % (MAX_PIPE_HEIGHT_GAP - PIPE_BITMAP_HEIGHT + 1) + PIPE_BITMAP_HEIGHT;
        this->x = current_x * (SH1106_HEIGHT - this->h);
    }
}

void single_pipe_init(Pipe *this,uint8_t first_pipe){
    if(first_pipe){
        current_x = PIPE_INITIAL_POS_X;
        current_y= PIPE_INITIAL_POS_Y;
    }

    this->w = PIPE_BITMAP_WIDTH;
    this->h = rand() % (MAX_PIPE_HEIGHT_GAP - PIPE_BITMAP_HEIGHT + 1) + PIPE_BITMAP_HEIGHT;

    this->x = current_x * (SH1106_HEIGHT - this->h);
    this->y = current_y;

    current_x = (current_x + 1) % 2;
    current_y = current_y + MIN_PIPE_WIDTH_GAP;
}

void pipe_init(Pipe *this){
    

    this->render = pipe_render;
    this->detect_collision = pipe_detect_collision;
    this->position_update = pipe_position_update;
    this->init = single_pipe_init;

    this->type = PIPE;
    
}



