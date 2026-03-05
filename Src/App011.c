/*
*   I2C Master sends data to Slave when a button is pressed!
*/


#include "stm32f4xx_gpio_driver.h"
#include "stm32f4xx_i2c_driver.h"
#include <string.h>
#include <stdio.h>

#define MASTER_ADDR 0x31
#define SLAVE_ADDR  0x68
#define MAX_BUFFER_SIZE 25

#define LOW 0
#define HIGH 1
#define I2C_DISABLE_SR 0
#define I2C_ENABLE_SR 1

// I2C1_SCL -> PB6
// I2C1_SDA -> PB7

extern void initialise_monitor_handles(void);


I2C_Handle_t I2CHandle1;


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


     // Button configuration
     GPIO_Handle_t BtnHandle;
     BtnHandle.pGPIOx = GPIOA;
     BtnHandle.GPIO_PinConfig.GPIO_PinNumber = 0;
     BtnHandle.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
     BtnHandle.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OUT_SPEED_HIGH;
     BtnHandle.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NO;
 
     GPIO_PeriClockControl(BtnHandle.pGPIOx, ENABLE);
     GPIO_Init(&BtnHandle);
}

// master i2c1 initialization
void I2C1_Init(){
    
    I2CHandle1.pI2Cx = I2C1;
    I2CHandle1.i2cConfig.I2C_SCLSpeed = I2C_SCL_SPEED_SM;
    I2CHandle1.i2cConfig.I2C_DeviceAddress = MASTER_ADDR;
    I2CHandle1.i2cConfig.I2C_ACKControl = I2C_ACK_ENABLE;
    I2C_Init(&I2CHandle1);
}

void wait_For_Btn_Press()
{
    while (GPIO_ReadFromInputPin(GPIOA, 0) == LOW)
        ;
    for (volatile int i = 0; i < 1000000; i++)
        ;
}



void  I2C_Communicatiion(){
    
    // enbale the I2C peripherals
    I2C_PeripheralControl(I2CHandle1.pI2Cx, ENABLE);
    
    uint8_t command = 0x51;

    // first send the length information to slave
    I2C_MasterSendData(&I2CHandle1, &command, 1, SLAVE_ADDR, I2C_ENABLE_SR);

   uint8_t len;
    // then receive the length information from slave
    I2C_MasterReceiveData(&I2CHandle1, &len, 1, SLAVE_ADDR, I2C_ENABLE_SR);

    // now send the command to slave to send the actual data
    command = 0x52;
    I2C_MasterSendData(&I2CHandle1, &command, 1, SLAVE_ADDR, I2C_ENABLE_SR);
    
    
    uint8_t rdata[MAX_BUFFER_SIZE];
    // now receive the actual data from slave
    I2C_MasterReceiveData(&I2CHandle1, rdata, len, SLAVE_ADDR, I2C_DISABLE_SR);
    rdata[len] = '\0'; // null termination
    iprintf("Data received from slave: %s\n", rdata);
    

}

int main(){
   
    init_Gpios();
    I2C1_Init();

    while(1){
        wait_For_Btn_Press();
        I2C_Communicatiion();
    }

    return 0;
}

