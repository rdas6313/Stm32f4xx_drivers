/*
*   I2C Master sends data to Slave when a button is pressed! Testing Interrupt based I2C communication!
*/


#include "stm32f4xx_gpio_driver.h"
#include "stm32f4xx_i2c_driver.h"
#include <string.h>
#include <stdio.h>

#define MASTER_ADDR 0x31
#define SLAVE_ADDR  0x68
#define MAX_BUFFER_SIZE 50

#define LOW 0
#define HIGH 1
#define I2C_DISABLE_SR 0
#define I2C_ENABLE_SR 1

// I2C1_SCL -> PB6
// I2C1_SDA -> PB7

extern void initialise_monitor_handles(void);


volatile I2C_Handle_t I2CHandle1;
uint8_t rdata[MAX_BUFFER_SIZE];
volatile uint8_t isdata_received = 0;
volatile uint8_t len;


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
    I2C_Init((I2C_Handle_t*)&I2CHandle1);
    // Enable the IRQs for I2C1
    I2C_IRQConfig(I2CHandle1.pI2Cx,I2C1_EV_IRQ, ENABLE);
    I2C_IRQConfig(I2CHandle1.pI2Cx,I2C1_ER_IRQ, ENABLE);
}

void wait_For_Btn_Press()
{
    while (GPIO_ReadFromInputPin(GPIOA, 0) == LOW)
        ;
    for (volatile int i = 0; i < 1000000; i++)
        ;
}

void I2C_AppCallback(I2C_Handle_t *pHandle, uint8_t status){
    if(status == I2C_EV_TX_COMPLETE){
        iprintf("Tx is completed\n");
    }else if(status == I2C_EV_RX_COMPLETE){
        isdata_received = 1;
        iprintf("Rx is completed\n");
    }else if(status == I2C_EV_STOP_DETECTED){
        iprintf("Stop is detected\n");
    }else if(status == I2C_ERROR_AF){
        iprintf("Error: Ack failure\n");
       
    }else if(status == I2C_ERROR_BERR){
        iprintf("Error: Bus error\n");
        
        // generate stop condition
        generate_stop_condition(pHandle->pI2Cx);
    }else if(status == I2C_ERROR_OVR){
        iprintf("Error: Overrun/Underrun\n");
       
        // generate stop condition
        generate_stop_condition(pHandle->pI2Cx);
    }else if(status == I2C_ERROR_ARLO){
        iprintf("Error: Arbitration lost\n");
       
        // generate stop condition
        generate_stop_condition(pHandle->pI2Cx);
    }else if(status == I2C_ERROR_TIMEOUT){
        iprintf("Error: Timeout error\n");
      
        // generate stop condition
        generate_stop_condition(pHandle->pI2Cx);
    }else if(status == I2C_EV_DATA_REQ){
        iprintf("Data request event\n");
    }else if(status == I2C_EV_DATA_RCV){
        iprintf("Data receive event\n");
    }else{
        iprintf("Unknown event\n");
    }
}

void  I2C_Communicatiion(){
    isdata_received = 0;

    // enbale the I2C peripherals
    I2C_PeripheralControl(I2CHandle1.pI2Cx, ENABLE);
    
    uint8_t command = 0x51;

    // first send the length information to slave
    while(I2C_MasterSendDataIT((I2C_Handle_t*)&I2CHandle1, &command, 1, SLAVE_ADDR, I2C_DISABLE_SR) != I2C_STATE_READY);
   
    // then receive the length information from slave
    while(I2C_MasterReceiveDataIT((I2C_Handle_t*)&I2CHandle1, (uint8_t*)&len, 1, SLAVE_ADDR, I2C_DISABLE_SR) != I2C_STATE_READY);

    // now send the command to slave to send the actual data
    command = 0x52;
    while(I2C_MasterSendDataIT((I2C_Handle_t*)&I2CHandle1, &command, 1, SLAVE_ADDR, I2C_DISABLE_SR) != I2C_STATE_READY);
    
    isdata_received = 0;
    
    // now receive the actual data from slave
    while(I2C_MasterReceiveDataIT((I2C_Handle_t*)&I2CHandle1, rdata, len, SLAVE_ADDR, I2C_DISABLE_SR) != I2C_STATE_READY);
    
    while(isdata_received == 0);

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

void I2C1_EV_IRQHandler(void){
    I2C_EV_IRQHandling((I2C_Handle_t*)&I2CHandle1);
}

void I2C1_ER_IRQHandler(void){
    I2C_ER_IRQHandling((I2C_Handle_t*)&I2CHandle1);
}



