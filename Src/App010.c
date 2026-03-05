/*
*   I2C Master sends data to Slave when a button is pressed!
*/


#include "stm32f4xx_gpio_driver.h"
#include "stm32f4xx_i2c_driver.h"
#include <string.h>

#define MASTER_ADDR 0x31
#define SLAVE_ADDR  0x68

#define LOW 0
#define HIGH 1
#define I2C_DISABLE_SR 0
#define I2C_ENABLE_SR 1

// I2C1_SCL -> PB6
// I2C1_SDA -> PB7

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
    uint8_t sdata[] = "Hello!";
    uint8_t sdata_len = strlen((char*)sdata);
    //uint8_t rdata[100];
    
    // enbale the I2C peripherals
    I2C_PeripheralControl(I2CHandle1.pI2Cx, ENABLE);
    I2C_MasterSendData(&I2CHandle1, sdata, sdata_len, SLAVE_ADDR, I2C_DISABLE_SR);
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















// void  I2C_Communicatiion(){
//     uint8_t sdata[] = "Hello!";
//     uint8_t sdata_len = strlen((char*)sdata);
//     uint8_t rdata[100];
//     // enbale the I2C peripherals
//     I2C_PeripheralControl(I2CHandle1.pI2Cx, ENABLE);
   
//     // generate start condition
//     generate_start_condition(I2CHandle1.pI2Cx);
//     // wait for SB flag set and clear by reading SR1 followed by writing to DR the slave address and slave address LSB bit should be 0 for write
//     wait_for_SB_flag(I2CHandle1.pI2Cx);
//     read_status_register(I2CHandle1.pI2Cx,I2C_READ_SR1);
//     send_slave_address_with_rw_bit(I2CHandle1.pI2Cx,SLAVE_ADDR,I2C_MASTER_WRITE);

//     /* Slave Wait for ADDR and clears it */
//     I2C_PeripheralControl(I2CHandle2.pI2Cx, ENABLE);
//     I2C_WaitForSlaveSelect(&I2CHandle2);
//     /* Slave end */

//     // wait for ADDR flag set and clear by reading SR1 followed by reading SR2
//     wait_for_ADDR_flag(I2CHandle1.pI2Cx);
//     read_status_register(I2CHandle1.pI2Cx,I2C_READ_SR1);
//     read_status_register(I2CHandle1.pI2Cx,I2C_READ_SR2);
    
//     for(int i=0;i<=sdata_len;i++){
//         // wait for TXE flag set and then write 8 bit data to DR until Len becomes 0
//         i2c_send_data(I2CHandle1.pI2Cx,&sdata[i],1);

//         // Slave side, wait until data is available to read
//         while(!I2C_SlaveDataAvailableForRead(I2CHandle2.pI2Cx));
//         // Slave reads data from data register
//         rdata[i] = I2C_SlaveReadData(I2CHandle2.pI2Cx);

//     }

//     // wait for TXE and BTF flag set and then generate stop condition
//     wait_for_TXE_BTF_flags(I2CHandle1.pI2Cx);
//     generate_stop_condition(I2CHandle1.pI2Cx);

//     // Slave side, wait until stop condition is detected and clears it
//     while(!I2C_SlaveEndReadDetected(I2CHandle2.pI2Cx));
//     I2C_SlaveEndRead(I2CHandle2.pI2Cx);
//     (void)rdata;

// }