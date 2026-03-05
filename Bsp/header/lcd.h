/*
*   LCD driver header file
*/

#ifndef LCD_H
#define LCD_H


#include "../../Drivers/Inc/stm32f407xx.h"
#include "../../Drivers/Inc/stm32f4xx_gpio_driver.h"
#include <string.h>



/* LCD PIN NO */
#define LCD_RS_PIN_NO           1
#define LCD_RW_PIN_NO           2
#define LCD_EN_PIN_NO           3
#define LCD_DATA7_PIN_NO        4
#define LCD_DATA6_PIN_NO        5
#define LCD_DATA5_PIN_NO        6
#define LCD_DATA4_PIN_NO        7

/* LCD PIN PORT */
#define LCD_PORT              GPIOA
#define LCD_RS_PORT           LCD_PORT
#define LCD_RW_PORT           LCD_PORT
#define LCD_EN_PORT           LCD_PORT
#define LCD_DATA7_PORT        LCD_PORT
#define LCD_DATA6_PORT        LCD_PORT
#define LCD_DATA5_PORT        LCD_PORT
#define LCD_DATA4_PORT        LCD_PORT


/* LCD Commands */
#define LCD_INIT_CMD1          0x03
#define LCD_INIT_CMD2          LCD_INIT_CMD1
#define LCD_INIT_CMD3          LCD_INIT_CMD1
#define LCD_INIT_CMD4          0x02
#define LCD_FUNCTION_SET_4BIT_2LINE  0x28
#define LCD_FUNCTION_SET_4BIT_1LINE  0x20
#define LCD_DISPLAY_OFF_CURSOR_OFF   0x08
#define LCD_DISPLAY_ON_CURSOR_ON     0x0E
#define LCD_DISPLAY_CLEAR            0x01
#define LCD_ENTRY_MODE_SET           0x06
#define LCD_DISPLAY_SHIFT_RIGHT      0x1C
#define LCD_DISPLAY_SHIFT_LEFT       0x18
#define LCD_FIRST_LINE_ADDR          0x00
#define LCD_SECOND_LINE_ADDR         0x40



/* LCD functions */
void LCD_SendCommand(uint8_t cmd);
void LCD_Init(void (*fDelay)(uint16_t delay)); // make sure to Init LCD GPIOs before calling this function
void LCD_Ddram_SetAddress(uint8_t address);
void LCD_Ddram_SendData(uint8_t data);
void LCD_SendString(char *str);

#endif