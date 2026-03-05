#include "../Bsp/header/ds1307.h"
#include "../Bsp/header/lcd.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx_timer.h"

static void Delay_ms(volatile uint16_t delay){
    Start_basic_timer(TIM6);
    while(delay > 0){
        while(!Get_Timer_update_status(TIM6)); // Wait for timer update event
        delay--;
    }
    Stop_basic_timer(TIM6);
}

static void Init_LCD_GPIOs(void){
    GPIO_Handle_t lcd_gpio_handle;

    memset(&lcd_gpio_handle,0,sizeof(lcd_gpio_handle));

    // LCD general pin configuration
    
    lcd_gpio_handle.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    lcd_gpio_handle.GPIO_PinConfig.GPIO_PinOPType = GPIO_OUT_PUSPLL;
    lcd_gpio_handle.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NO;
    lcd_gpio_handle.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OUT_SPEED_HIGH;

    // Enable LCD GPIO clock
    lcd_gpio_handle.pGPIOx = LCD_PORT;
    GPIO_PeriClockControl(lcd_gpio_handle.pGPIOx, ENABLE);
    

    // LCD RS pin configuration
    lcd_gpio_handle.GPIO_PinConfig.GPIO_PinNumber = LCD_RS_PIN_NO;
    GPIO_Init(&lcd_gpio_handle);

    // LCD RW pin configuration
    lcd_gpio_handle.GPIO_PinConfig.GPIO_PinNumber = LCD_RW_PIN_NO;
    GPIO_Init(&lcd_gpio_handle);

    // LCD EN pin configuration
    lcd_gpio_handle.GPIO_PinConfig.GPIO_PinNumber = LCD_EN_PIN_NO;
    GPIO_Init(&lcd_gpio_handle);

    // LCD DATA7 pin configuration
    lcd_gpio_handle.GPIO_PinConfig.GPIO_PinNumber = LCD_DATA7_PIN_NO;
    GPIO_Init(&lcd_gpio_handle);

    // LCD DATA6 pin configuration
    lcd_gpio_handle.GPIO_PinConfig.GPIO_PinNumber = LCD_DATA6_PIN_NO;
    GPIO_Init(&lcd_gpio_handle);

    // LCD DATA5 pin configuration
    lcd_gpio_handle.GPIO_PinConfig.GPIO_PinNumber = LCD_DATA5_PIN_NO;
    GPIO_Init(&lcd_gpio_handle);

    // LCD DATA4 pin configuration
    lcd_gpio_handle.GPIO_PinConfig.GPIO_PinNumber = LCD_DATA4_PIN_NO;
    GPIO_Init(&lcd_gpio_handle);

}

void Init_Timer(){
    uint16_t prescaler = 100 -1;
    uint16_t arr = 160 - 1; // 1ms delay at 16MHz clock with prescaler of 100
    Init_basic_timer(TIM6, prescaler, arr);
    
}

// void Test(){
//     GPIO_Handle_t timer_test_gpio_handle;

//     memset(&timer_test_gpio_handle,0,sizeof(timer_test_gpio_handle));
    
//     timer_test_gpio_handle.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
//     timer_test_gpio_handle.GPIO_PinConfig.GPIO_PinOPType = GPIO_OUT_PUSPLL;
//     timer_test_gpio_handle.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NO;
//     timer_test_gpio_handle.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OUT_SPEED_HIGH;

//     timer_test_gpio_handle.pGPIOx = GPIOA;
//     GPIO_PeriClockControl(timer_test_gpio_handle.pGPIOx, ENABLE);
    

//     // LCD RS pin configuration
//     timer_test_gpio_handle.GPIO_PinConfig.GPIO_PinNumber = 0;
//     GPIO_Init(&timer_test_gpio_handle);

//     uint16_t delay = 20; // 20ms delay

//     Start_basic_timer(TIM6);
//     while(delay > 0){
//         GPIO_ToggleOuputPin(GPIOA, 0);
//         while(!Get_Timer_update_status(TIM6)); // Wait for timer update event
//         delay--;
//         GPIO_ToggleOuputPin(GPIOA, 0);
//     }
//     Stop_basic_timer(TIM6);
// }

int main(){

    Init_Timer();

    
    Init_LCD_GPIOs();

    LCD_Init(&Delay_ms);
    
    LCD_Ddram_SetAddress(LCD_FIRST_LINE_ADDR);
    LCD_SendString("Miles to go before I sleep...");
    LCD_Ddram_SetAddress(LCD_SECOND_LINE_ADDR);
    LCD_SendString("          -- Robert Frost");
    Delay_ms(2000);
    while(1){
        LCD_SendCommand(LCD_DISPLAY_SHIFT_LEFT);
        Delay_ms(500);
    }
    
    return 0;
}