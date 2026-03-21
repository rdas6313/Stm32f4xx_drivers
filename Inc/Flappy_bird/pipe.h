#ifndef PIPE_H
#define PIPE_H

#include "sh1106_graphics.h"
#include "bird.h"


#define PIPE_INITIAL_POS_X 0
#define PIPE_INITIAL_POS_Y 128
#define PIPE_BITMAP_WIDTH 8
#define PIPE_BITMAP_HEIGHT 20
#define MAX_PIPE_HEIGHT_GAP (SH1106_HEIGHT - BIRD_BITMAP_HEIGHT - 12)
#define MIN_PIPE_WIDTH_GAP  (6 * PIPE_BITMAP_WIDTH)

typedef struct Pipe{
    
    SceneType type;

    uint8_t x;
    uint8_t y;
    uint8_t w;
    uint8_t h;

    void (*init)(struct Pipe *this,uint8_t first_pipe);
    void (*render)(struct Pipe *this);
    uint8_t (*detect_collision)(struct Pipe *this,struct Bird *bird);
    void (*position_update)(struct Pipe *this);
} Pipe;


void pipe_init(Pipe *this);

#endif