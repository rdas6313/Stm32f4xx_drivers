/*
*   I2C Master sends data to Slave when a button is pressed!
*/


#include "../Bsp/header/sh1106.h"
#include "stm32f4xx_gpio_driver.h"
#include "stm32f4xx_i2c_driver.h"
#include "stm32f4xx_timer.h"
#include <string.h>
#include <stdio.h>

#define MASTER_ADDR 0x31
#define SLAVE_ADDR  SH1106_I2C_ADDRESS
#define MAX_BUFFER_SIZE 25

#define LOW 0
#define HIGH 1
#define I2C_DISABLE_SR 0
#define I2C_ENABLE_SR 1

// I2C1_SCL -> PB6
// I2C1_SDA -> PB7

extern void initialise_monitor_handles(void);



/* Dummy Image */

extern const unsigned char lion [];
extern const unsigned char maa [] ;
/* Dummy Image end */





I2C_Handle_t I2CHandle1;

static void Delay_ms(volatile uint16_t delay){
    Start_basic_timer(TIM6);
    while(delay > 0){
        while(!Get_Timer_update_status(TIM6)); // Wait for timer update event
        delay--;
    }
    Stop_basic_timer(TIM6);
}

void Init_Timer(){
    uint16_t prescaler = 100 -1;
    uint16_t arr = 160 - 1; // 1ms delay at 16MHz clock with prescaler of 100
    Init_basic_timer(TIM6, prescaler, arr);
    
}

void init_Gpios(){
    GPIO_Handle_t I2CGpioHandle1;
    I2CGpioHandle1.pGPIOx = GPIOB;
    I2CGpioHandle1.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    I2CGpioHandle1.GPIO_PinConfig.GPIO_PinAltFunMode = GPIO_AF_4;
    I2CGpioHandle1.GPIO_PinConfig.GPIO_PinOPType = GPIO_OUT_OD;
    I2CGpioHandle1.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NO;
    I2CGpioHandle1.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OUT_SPEED_HIGH;
    // enable GPIOB clock
    GPIO_PeriClockControl(GPIOB, ENABLE);

    // I2C1_SCL
    I2CGpioHandle1.GPIO_PinConfig.GPIO_PinNumber = 6;
    GPIO_Init(&I2CGpioHandle1);
    // I2C1_SDA
    I2CGpioHandle1.GPIO_PinConfig.GPIO_PinNumber = 7;
    GPIO_Init(&I2CGpioHandle1);
}

// master i2c1 initialization
void I2C1_Init(){
    
    I2CHandle1.pI2Cx = I2C1;
    I2CHandle1.i2cConfig.I2C_SCLSpeed = I2C_SCL_SPEED_FM;
    I2CHandle1.i2cConfig.I2C_DeviceAddress = MASTER_ADDR;
    I2CHandle1.i2cConfig.I2C_ACKControl = I2C_ACK_ENABLE;
    I2C_Init(&I2CHandle1);
}


void i2c_send_SH1106(uint8_t address,uint8_t *data, uint16_t size){
    I2C_MasterSendData(&I2CHandle1, data, size, SLAVE_ADDR, I2C_DISABLE_SR);
}

void delay(uint16_t ms){
    Delay_ms(ms);
}

int main(){
   
    init_Gpios();
    Init_Timer();
    I2C1_Init();

    // enbale the I2C peripherals
    I2C_PeripheralControl(I2CHandle1.pI2Cx, ENABLE);

    SH1106_init();

    while(1){
        SH1106_show((uint8_t*)lion);
        for(int i=0;i<64;i++){
            SH1106_set_display_line(i);
            Delay_ms(1000);
        }

        for(int i=63;i>=0;i--){
            SH1106_set_display_line(i);
            Delay_ms(1000);
        }
    }

    
   
    return 0;
}

