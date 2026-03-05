
/*
*   SH1106 Controller driver header file
*/

#ifndef SH1106_H
#define SH1106_H

#include<string.h>
#include<stdint.h>

#define SH1106_I2C_ADDRESS 0x3C // I2C address for SH1106 display
#define SH1106_WIDTH 128
#define SH1106_HEIGHT 64
#define SH1106_PAGE_MAX 8

// default values for all commands to initialize the display
#define SH1106_LOWER_COL_DEFAULT 0x00
#define SH1106_HIGHER_COL_DEFAULT 0x10
#define SH1106_PUMP_VOLTAGE_DEFAULT 0x32
#define SH1106_DISPLAY_START_LINE_DEFAULT 0x40
#define SH1106_CONTRAST_DEFAULT 0x80
#define SH1106_SEGMENT_REMAP_DEFAULT 0xA1
#define SH1106_ENTIRE_DISPLAY_DEFAULT 0xA4
#define SH1106_NORMAL_INVERSE_DEFAULT 0xA6
#define SH1106_MULTIPLEX_RATIO_DEFAULT 0x3F
#define SH1106_DC_DC_DEFAULT 0x8B
#define SH1106_DISPLAY_OFF_DEFAULT 0xAE
#define SH1106_PAGE_START_ADDRESS_DEFAULT 0xB0
#define SH1106_COM_SCAN_DIR_DEFAULT 0xC8
#define SH1106_DISPLAY_OFFSET_DEFAULT 0x00
#define SH1106_FREQUENCY_DEFAULT 0x50
#define SH1106_PRE_DIS_CHARGE_DEFAULT 0x22
#define SH1106_COMMON_PADS_DEFAULT 0x12
#define SH1106_VCOMH_DESELECT_LEVEL_DEFAULT 0x35


// SH1106 command definitions
#define SH1106_COMMAND_CONTROL 0x00
#define SH1106_DATA_CONTROL 0x40
#define SH1106_CONTRAST_COMMAND 0x81
#define SH1106_MULTIPLEX_RATIO_COMMAND 0xA8
#define SH1106_DC_DC_COMMAND 0xAD
#define SH1106_DISPLAY_OFFSET_COMMAND 0xD3
#define SH1106_FREQUENCY_COMMAND 0xD5
#define SH1106_PRE_DIS_CHARGE_COMMAND 0xD9
#define SH1106_COMMON_PADS_COMMAND 0xDA
#define SH1106_VCOMH_DESELECT_LEVEL_COMMAND 0xDB
#define SH1106_DISPLAY_ON_OFF_COMMAND 0xAE
#define SH1106_COLUMN_LOWER_ADDRESS_COMMAND 0x00
#define SH1106_COLUMN_HIGHER_ADDRESS_COMMAND 0x10
#define SH1106_PAGE_ADDRESS_COMMAND 0xB0

#define SH1106_DISPLAY_ON 1
#define SH1106_DISPLAY_OFF 0

/* * * * * * * * * * * * * * * * *  * * * * * 
*   Public functions for SH1106 OLED display      *
** * * * * * * * * * * * * * * *  * * * * * */

void SH1106_init(void);
void SH1106_show(uint8_t *data);
void SH1106_clear(void);
void SH1106_display_on_off(uint8_t on);
// void SH1106_set_display_line(uint8_t line);
// void SH1106_change_orientation(uint8_t horizontal);
// void SH1106_test_display(void);
// void SH1106_set_brightness(uint8_t brightness);

void i2c_send_SH1106(uint8_t address,uint8_t *data, uint16_t size);
void delay(uint16_t ms);



#endif

































