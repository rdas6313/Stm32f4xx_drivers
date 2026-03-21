#ifndef FLOOR_H
#define FLOOR_H


#define UP_FLOOR_INITIAL_POS_X 0
#define UP_FLOOR_INITIAL_POS_Y 0
#define UP_FLOOR_WIDTH 128
#define UP_FLOOR_HEIGHT 1

#define DOWN_FLOOR_INITIAL_POS_X 63
#define DOWN_FLOOR_INITIAL_POS_Y 0
#define DOWN_FLOOR_WIDTH 128
#define DOWN_FLOOR_HEIGHT 1


typedef struct Floor{

    SceneType type;

    uint8_t x;
    uint8_t y;
    uint8_t w;
    uint8_t h;

    void (*init)(struct Floor *this,uint8_t is_up);
    void (*render)(struct Floor *this);

} Floor;

void floor_init(Floor *this);

#endif