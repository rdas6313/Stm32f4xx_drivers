#ifndef SH1106_GRAPHICS_H
#define SH1106_GRAPHICS_H


#include "sh1106.h"
#include<stdlib.h>
#include<string.h>



/* * * * * * * * * * * * * * * * *  * * * * * 
* Macros for SH1106 Graphics Lib Internal use*
** * * * * * * * * * * * * * * *  * * * * * */

#define swap(x,y) do{int32_t tmp = x; x = y; y = tmp;}while(0)
#define abs(x) ((x) < 0 ? -(x):(x))
#define valid_height(x) ((x) < SH1106_HEIGHT)
#define valid_width(x) ((x) < SH1106_WIDTH)
#define min(a,b) ((a) < (b) ? (a) : (b))

#define FONT_WIDTH 5
#define FONT_HEIGHT 7

/* * * * * * * * * * * * * * * * *  * * * * * 
*  Public functions for SH1106 Graphics Lib *
** * * * * * * * * * * * * * * *  * * * * * */


void gfx_init(void);



void gfx_clear_buffer();



void gfx_update_display();



void gfx_draw_pixel(uint8_t x,uint8_t y, uint8_t color);



void gfx_draw_line(uint8_t x0,uint8_t y0, uint8_t x1,uint8_t y1,uint8_t color);



void gfx_draw_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h,uint8_t color);



void gfx_draw_fill_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h,uint8_t color);


void gfx_draw_char(uint8_t x, uint8_t y, uint8_t c,uint8_t size,uint8_t color);



void gfx_draw_string(uint8_t x, uint8_t y, const char *str,const uint8_t size);



void gfx_draw_bitmap(uint8_t x, uint8_t y, const uint8_t *bmp, uint8_t w, uint8_t h);



void gfx_draw_hline(uint8_t x, uint8_t y, uint8_t w);

void gfx_draw_vline(uint8_t x, uint8_t y, uint8_t h);






#endif