
#include "sh1106_graphics.h"


static uint8_t frame_buffer[SH1106_PAGE_MAX][SH1106_WIDTH];


// init graphics system
void gfx_init(void){
    memset(frame_buffer,0,sizeof(frame_buffer));
    SH1106_init();
}

// clear framebuffer
void gfx_clear_buffer(){
    memset(frame_buffer,0,sizeof(frame_buffer));
}


// push framebuffer to sh1106 controller
void gfx_update_display(){
    SH1106_show((uint8_t*)frame_buffer);
}

// draw a pixel in the specified location (x,y) of specified color(1 or 0).
void gfx_draw_pixel(uint8_t x,uint8_t y, uint8_t color){
    if(x >= SH1106_HEIGHT || y >= SH1106_WIDTH)
        return;

    uint8_t page = x / SH1106_BIT_PER_PAGE;
    uint8_t bit = x % SH1106_BIT_PER_PAGE;

    frame_buffer[page][y] &= (~(1 << bit));
    frame_buffer[page][y] |= ((color & 0x01) << bit);
}

#define swap(x,y) do{int32_t tmp = x; x = y; y = tmp;}while(0)
#define abs(x) ((x) < 0 ? -(x):(x))
#define valid_height(x) ((x) < SH1106_HEIGHT)
#define valid_width(x) ((x) < SH1106_WIDTH)
#define min(a,b) ((a) < (b) ? (a) : (b))

// implemented Bresenham's line drawing algorithm.
void gfx_draw_line( uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color){

    if(!valid_height(x0) || !valid_height(x1) || !valid_width(y0) || !valid_width(y1))
        return;
    

    int16_t dy = abs(y1 - y0);
    int16_t dx = abs(x1 - x0);

    uint8_t f = 0;

    if(dy > dx){
        
        f = 1;

        swap(x0,y0);
        
        swap(x1,y1);

        swap(dx,dy);
        
    }

    uint8_t x = x0;
    uint8_t y = y0;
    int16_t p = 2 * dy - dx;

    int16_t xm = ((x1-x0) < 0) ? -1 : 1;
    int16_t ym = ((y1-y0) < 0) ? -1 : 1;

    while(1){

        if(f)
            gfx_draw_pixel(y,x,1);
        else
            gfx_draw_pixel(x,y,1);

        if(x == x1 && y == y1)
            break;

        if(p < 0){
            p = p + 2 * dy; 
        }else{
            y += ym;
            p = p + (2 * dy) - (2 * dx);
        }

        x += xm;
    }

}

void gfx_draw_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h,uint8_t color){

    if(!valid_height(x) || !valid_width(y) || w == 0 || w > SH1106_WIDTH || h == 0 || h > SH1106_HEIGHT)
        return;

    uint8_t x1 = x;
    uint8_t y1 = min(y + (w-1),SH1106_WIDTH - 1);

    uint8_t x2 = min(x + (h-1),SH1106_HEIGHT - 1);
    uint8_t y2 = y;

    uint8_t x3 = min(x + (h-1),SH1106_HEIGHT - 1);
    uint8_t y3 = min(y + (w-1),SH1106_WIDTH - 1);

    gfx_draw_line(x,y,x1,y1,color);
    gfx_draw_line(x,y,x2,y2,color);
    gfx_draw_line(x2,y2,x3,y3,color);
    gfx_draw_line(x3,y3,x1,y1,color);

}

void gfx_draw_fill_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h){

}

void gfx_draw_char(uint8_t x, uint8_t y, char c){

}

void gfx_draw_string(uint8_t x, uint8_t y, const char *str){

}


void gfx_draw_bitmap(uint8_t x, uint8_t y, const uint8_t *bmp, uint8_t w, uint8_t h){

}


// horizontal line
void gfx_draw_hline(uint8_t x, uint8_t y, uint8_t w){

}

// vertical line
void gfx_draw_vline(uint8_t x, uint8_t y, uint8_t h){

}
