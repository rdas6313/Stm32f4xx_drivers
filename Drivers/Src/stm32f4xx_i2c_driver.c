#include "stm32f4xx_i2c_driver.h"

static void i2c_send_data(I2C_RegDef_t *pI2Cx,uint8_t *pTxBuffer,uint32_t Len);
static void wait_for_TXE_BTF_flags(I2C_RegDef_t *pI2Cx);
static void wait_for_SB_flag(I2C_RegDef_t *pI2Cx);
static void wait_for_ADDR_flag(I2C_RegDef_t *pI2Cx);
static void send_slave_address_with_rw_bit(I2C_RegDef_t *pI2Cx,uint8_t SlaveAddr,uint8_t rw_bit);

/**
 * @brief This function helps to enable or disable i2c peripheral clock to enable or disable entire i2c
 * @param param1 pointer of I2C base register
 * @param param2 Enable or disable
 * @return None
 */
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi)
{
    if (EnOrDi)
    {
        if (pI2Cx == I2C1)
            I2C1_PCLK_EN();
        else if (pI2Cx == I2C2)
            I2C2_PCLK_EN();
        else if (pI2Cx == I2C3)
            I2C3_PCLK_EN();
    }
    else
    {
        if (pI2Cx == I2C1)
            I2C1_PCLK_DI();
        else if (pI2Cx == I2C2)
            I2C2_PCLK_DI();
        else if (pI2Cx == I2C3)
            I2C3_PCLK_DI();
    }
}

/**
 * @brief This function helps to initialize I2C peripheral
 * @param param1 pointer of I2C Handle
 * @return None
 */
void I2C_Init(I2C_Handle_t *pI2CHandle)
{
    // enable clock for I2C peripheral
    I2C_PeriClockControl(pI2CHandle->pI2Cx, ENABLE);

    //  Configure Frequency
    uint32_t peripheral_clk = Get_Peripheral_Clock(1);
    uint32_t freq = peripheral_clk / MHz;
    pI2CHandle->pI2Cx->CR2 |= (freq & 0x3F);

    // Configure address
    pI2CHandle->pI2Cx->OAR1 = 0x0;
    pI2CHandle->pI2Cx->OAR1 &= ~(1 << 15);
    pI2CHandle->pI2Cx->OAR1 |= (1 << 14);
    pI2CHandle->pI2Cx->OAR1 |= (pI2CHandle->i2cConfig.I2C_DeviceAddress << 1);

    // configure SCL
    uint32_t ccr;
    if (pI2CHandle->i2cConfig.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
    {
        // standard mode selection
        pI2CHandle->pI2Cx->CCR &= ~(1 << 15);
        // configure ccr
        ccr = peripheral_clk / (pI2CHandle->i2cConfig.I2C_SCLSpeed * 2);
    }
    else
    {
        // Fast mode selection
        pI2CHandle->pI2Cx->CCR |= (1 << 15);

        // configure duty
        pI2CHandle->pI2Cx->CCR &= ~(1 << 14);
        pI2CHandle->pI2Cx->CCR |= (pI2CHandle->i2cConfig.I2C_FMDutyCycle << 14);

        // configure ccr
        if (pI2CHandle->i2cConfig.I2C_FMDutyCycle == I2C_FM_DUTY_2)
        {
            ccr = peripheral_clk / (3 * pI2CHandle->i2cConfig.I2C_SCLSpeed);
        }
        else
        {
            ccr = peripheral_clk / (25 * pI2CHandle->i2cConfig.I2C_SCLSpeed);
        }
    }
    pI2CHandle->pI2Cx->CCR |= (ccr & 0xFFF);

    // configure TRISE
    if(pI2CHandle->i2cConfig.I2C_SCLSpeed <= I2C_SCL_SPEED_SM){
        // trise for standard mode 1000ns
        uint8_t trise = (peripheral_clk / MHz) + 1;
        pI2CHandle->pI2Cx->TRISE = (trise & 0x3F);

    }else{
        // trise for fast mode 300ns
        pI2CHandle->pI2Cx->TRISE = ((((peripheral_clk * 300) / GHz) + 1) & 0x3F);

    }

    I2C_PeripheralControl(pI2CHandle->pI2Cx, ENABLE); // Enable the peripheral during configuration

    // Configure Ack Control
    pI2CHandle->pI2Cx->CR1 &= ~(1 << 10);
    pI2CHandle->pI2Cx->CR1 |= (pI2CHandle->i2cConfig.I2C_ACKControl << 10);

    // I2C_PeripheralControl(pI2CHandle->pI2Cx, DISABLE); // disable the peripheral during configuration

    // reset TxRxState
    pI2CHandle->TxRxState = I2C_STATE_READY;
}

/**
 * @brief This function helps to reset I2C peripheral registers
 * @param param1 pointer of I2C base register
 * @return None
 */
void I2C_DeInit(I2C_RegDef_t *pI2Cx)
{
    if (pI2Cx == I2C1)
    {
        RCC->APB1RSTR |= (1 << 21);
    }
    else if (pI2Cx == I2C2)
    {
        RCC->APB1RSTR |= (1 << 22);
    }
    else if (pI2Cx == I2C3)
    {
        RCC->APB1RSTR |= (1 << 23);
    }
}

/**
 * @brief This function helps to Enable or Disable I2C peripheral
 * @param param1 pointer of I2C base register
 * @param param2 Enable or disable
 * @return None
 */
void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi)
{
    if (EnOrDi)
    {
        pI2Cx->CR1 |= (1 << 0);
    }
    else
    {
        pI2Cx->CR1 &= ~(1 << 0);
    }
}

/*
* @brief This function helps to Enable or Disable I2C IRQ of NVIC
* @param param1 pointer of I2C base register
* @param param2 IRQ number
* @param param3 Enable or disable
* @return None
*/
void I2C_IRQConfig(I2C_RegDef_t *pI2Cx,uint8_t IRQNumber, uint8_t EnOrDi)
{
    if(EnOrDi){
        NVIC_ENABLE_IRQ(IRQNumber);
      
    }
    else{
        NVIC_DISABLE_IRQ(IRQNumber);
       
    }
}

/*
* @brief This function helps to Enable or Disable I2C Interrupt signals
* @param param1 pointer of I2C Handle
* @param param2 Enable or disable
* @return None
*/
void I2C_ManageInterrupt(I2C_Handle_t *pI2CHandle,uint8_t EnOrDi){
    if(EnOrDi){
        // enable ITBUFEN, ITEVTEN and ITERREN control bits
        pI2CHandle->pI2Cx->CR2 |= (1 << I2C_ITERREN);
        pI2CHandle->pI2Cx->CR2 |= (1 << I2C_ITEVTEN);
        pI2CHandle->pI2Cx->CR2 |= (1 << I2C_ITBUFEN);
    }else{
        // stop all interrupts
        pI2CHandle->pI2Cx->CR2 &= ~(1<<I2C_ITEVTEN);
        pI2CHandle->pI2Cx->CR2 &= ~(1<<I2C_ITBUFEN);
        pI2CHandle->pI2Cx->CR2 &= ~(1<<I2C_ITERREN);
    }
}
/*
* @brief This function helps to configure IRQ priority
* @param param1 IRQ number
* @param param2 IRQ priority
* @return None
*/
void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
{
    NVIC_SET_PRIORITY(IRQNumber, IRQPriority);
}

/** 
* @brief This function helps to read status register flag
* @param param1 pointer of I2C base register
* @param param2 status flag to be read. E.g.- I2C_FLAG_SB, I2C_FLAG_ADDR etc
* @return flag status 1 or 0
*/
// uint8_t I2C_StatusFlag(uint32_t reg, uint16_t status_flag)
// {
//     return (reg & status_flag) ? 1 : 0;
// }

// Application callback function
__attribute__((weak))
void I2C_AppCallback(I2C_Handle_t *pHandle, uint8_t status)
{
}

/**
* @brief start condition generation
* @param param1 pointer of I2C base register
* @return None
*/
void generate_start_condition(I2C_RegDef_t *pI2Cx){
    while(I2C_StatusFlag(pI2Cx->SR2,I2C_FLAG_BUSY) && I2C_StatusFlag(pI2Cx->SR2,I2C_FLAG_MSL) == I2C_MODE_SLAVE); // wait until bus is free
    pI2Cx->CR1 |= (1<<8); // set start bit
}

/**
* @brief wait for SB flag set
* @param param1 pointer of I2C base register
* @return None
*/
void wait_for_SB_flag(I2C_RegDef_t *pI2Cx){
    while(I2C_StatusFlag(pI2Cx->SR1,I2C_FLAG_SB) == 0); // wait until SB flag is set
}

/**
* @brief wait for ADDR flag set
* @param param1 pointer of I2C base register
* @return None
*/
void wait_for_ADDR_flag(I2C_RegDef_t *pI2Cx){
    while(I2C_StatusFlag(pI2Cx->SR1,I2C_FLAG_ADDR) == 0); // wait until ADDR flag is set
}

/**
* @brief send slave address with R/W bit
* @param param1 pointer of I2C base register
* @param param2 slave address
* @param param3 R/W bit, 0 for write, 1 for read
* @return None
*/
void send_slave_address_with_rw_bit(I2C_RegDef_t *pI2Cx,uint8_t SlaveAddr,uint8_t rw_bit){
    SlaveAddr = SlaveAddr << 1; // left shift by 1 to make space for R/W bit
    SlaveAddr |= (rw_bit & 0x1); // set R/W bit
    pI2Cx->DR = SlaveAddr; // write address to data register
}

/**
* @brief read status register according to flag type, I2C_READ_SR1 or I2C_READ_SR2
* @param param1 pointer of I2C base register
* @param param2 flag type, I2C_READ_SR1 or I2C_READ_SR2
* @return None 
*/
void read_status_register(I2C_RegDef_t *pI2Cx,uint8_t flag_type){
    uint32_t tmp;
    if(flag_type == I2C_READ_SR1)
        tmp = pI2Cx->SR1;
    else
        tmp = pI2Cx->SR2;

    (void)tmp; // to avoid unused variable warning
}

/**
* @brief send data until Len becomes 0
* @param param1 pointer of I2C base register
* @param param2 pointer of Tx buffer
* @param param3 length of data
* @return None
*/
void i2c_send_data(I2C_RegDef_t *pI2Cx,uint8_t *pTxBuffer,uint32_t Len){
    while(Len > 0){
        while(I2C_StatusFlag(pI2Cx->SR1,I2C_FLAG_TXE) == 0); // wait until TXE flag is set
        pI2Cx->DR = *pTxBuffer; // write data to data register
        pTxBuffer++;
        Len--;
    }
}

/**
* @brief wait for TXE and BTF flags set
* @param param1 pointer of I2C base register
* @return None
*/
void wait_for_TXE_BTF_flags(I2C_RegDef_t *pI2Cx){
    while(I2C_StatusFlag(pI2Cx->SR1,I2C_FLAG_TXE) == 0); // wait until TXE flag is set
    while(I2C_StatusFlag(pI2Cx->SR1,I2C_FLAG_BTF) == 0); // wait until BTF flag is set
}

/**
* @brief stop condition generation
* @param param1 pointer of I2C base register
* @return None
*/
void generate_stop_condition(I2C_RegDef_t *pI2Cx){
    pI2Cx->CR1 |= (1<<9); // set stop bit
}

/**
* @brief This function helps master to send data
* @param param1 pointer of I2C Handle
* @param param2 pointer of Tx buffer
* @param param3 length of data
* @param param4 slave address
* @return None
*/
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t repeatedStart){
    // generate start condition
    generate_start_condition(pI2CHandle->pI2Cx);
    // wait for SB flag set and clear by reading SR1 followed by writing to DR the slave address and slave address LSB bit should be 0 for write
    wait_for_SB_flag(pI2CHandle->pI2Cx);
    read_status_register(pI2CHandle->pI2Cx,I2C_READ_SR1);
    send_slave_address_with_rw_bit(pI2CHandle->pI2Cx,SlaveAddr,I2C_MASTER_WRITE);
    // wait for ADDR flag set and clear by reading SR1 followed by reading SR2
    wait_for_ADDR_flag(pI2CHandle->pI2Cx);
    read_status_register(pI2CHandle->pI2Cx,I2C_READ_SR1);
    read_status_register(pI2CHandle->pI2Cx,I2C_READ_SR2);
    // wait for TXE flag set and then write 8 bit data to DR until Len becomes 0
    i2c_send_data(pI2CHandle->pI2Cx,pTxBuffer,Len);
    // wait for TXE and BTF flag set and then generate stop condition
    wait_for_TXE_BTF_flags(pI2CHandle->pI2Cx);
    if(!repeatedStart)
        generate_stop_condition(pI2CHandle->pI2Cx);
}

/**
* @brief Enable or disable ACKing
* @param param1 pointer of I2C base register
* @param param2 Enable or disable
* @return None
*/
void I2C_ChangeAck(I2C_RegDef_t *pI2Cx,uint8_t EnOrDi){
    if(EnOrDi)
        pI2Cx->CR1 |= (1<<I2C_ACK); // set ACK bit
    else
        pI2Cx->CR1 &= (~(1<<I2C_ACK)); // clear ACK bit
}

/**
* @brief wait for RXNE flag set
* @param param1 pointer of I2C base register
* @return None
*/
static void wait_for_RXNE_flag(I2C_RegDef_t *pI2Cx){
    while(I2C_StatusFlag(pI2Cx->SR1,I2C_FLAG_RXNE) == 0); // wait until RXNE flag is set
}

/**
* @brief This function helps master to receive data
* @param param1 pointer of I2C Handle
* @param param2 pointer of Rx buffer
* @param param3 length of data
* @param param4 slave address
* @return None
*/
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t repeatedStart)
{
    // generate start condition
    generate_start_condition(pI2CHandle->pI2Cx);
    // wait for SB flag set and clear by reading SR1 followed by writing to DR the slave address and slave address LSB bit should be 0 for write
    wait_for_SB_flag(pI2CHandle->pI2Cx);
    read_status_register(pI2CHandle->pI2Cx,I2C_READ_SR1);
    send_slave_address_with_rw_bit(pI2CHandle->pI2Cx,SlaveAddr,I2C_MASTER_READ);
    // wait for ADDR flag set 
    wait_for_ADDR_flag(pI2CHandle->pI2Cx);

    if(Len == 1){
        // disable ack
        I2C_ChangeAck(pI2CHandle->pI2Cx,I2C_ACK_DISABLE);
        // clear ADDR flag
        read_status_register(pI2CHandle->pI2Cx,I2C_READ_SR1);
        read_status_register(pI2CHandle->pI2Cx,I2C_READ_SR2);

        if(!repeatedStart){
            // generate stop condition
            generate_stop_condition(pI2CHandle->pI2Cx);
        }
        // wait until RXNE becomes 1
        wait_for_RXNE_flag(pI2CHandle->pI2Cx);
        // read data in rx buffer
        *pRxBuffer = pI2CHandle->pI2Cx->DR;
        
    }else{
        // clear ADDR flag
        read_status_register(pI2CHandle->pI2Cx,I2C_READ_SR1);
        read_status_register(pI2CHandle->pI2Cx,I2C_READ_SR2);

        // Receive data when receiver not empty, until Len becomes 2 and then Disable ack and generate stop condition and then read last two bytes
        while(Len > 0){
            wait_for_RXNE_flag(pI2CHandle->pI2Cx);
            if(Len == 2){
                I2C_ChangeAck(pI2CHandle->pI2Cx,I2C_ACK_DISABLE);
                if(!repeatedStart){
                    generate_stop_condition(pI2CHandle->pI2Cx);
                }
            }
            *pRxBuffer = pI2CHandle->pI2Cx->DR;
            Len--;
            pRxBuffer++;
        }
    }

    // Enable ack if it was initially enabled
    if(pI2CHandle->i2cConfig.I2C_ACKControl == I2C_ACK_ENABLE)
        I2C_ChangeAck(pI2CHandle->pI2Cx,I2C_ACK_ENABLE);

}


/**
*   @brief wait for slave select by master
*   @param param1 pointer of I2C base register
*   @return None
*/
void I2C_WaitForSlaveSelect(I2C_Handle_t *pI2CHandle){
    wait_for_ADDR_flag(pI2CHandle->pI2Cx);
    read_status_register(pI2CHandle->pI2Cx,I2C_READ_SR1);
    read_status_register(pI2CHandle->pI2Cx,I2C_READ_SR2);
}

/**
*   @brief check if slave data is available to read
*   @param param1 pointer of I2C base register  
*   @return flag status 1 or 0
*/
uint8_t I2C_SlaveDataAvailableForRead(I2C_RegDef_t *pI2Cx){
    return I2C_StatusFlag(pI2Cx->SR1,I2C_FLAG_RXNE);
}

/**
*   @brief slave reads data from data register
*   @param param1 pointer of I2C base register  
*   @return data byte
*/
uint8_t I2C_SlaveReadData(I2C_RegDef_t *pI2Cx){
    return (uint8_t)pI2Cx->DR;
}

/**
*   @brief check if stop condition is detected by slave
*   @param param1 pointer of I2C base register  
*   @return flag status 1 or 0
*/
uint8_t I2C_SlaveEndReadDetected(I2C_RegDef_t *pI2Cx){
    return I2C_StatusFlag(pI2Cx->SR1,I2C_FLAG_STOPF);
}

/**
*   @brief clear stop condition flag by reading SR1 and writing to CR1
*   @param param1 pointer of I2C base register  
*   @return None
*/
void I2C_SlaveEndRead(I2C_RegDef_t *pI2Cx){
    read_status_register(pI2Cx,I2C_READ_SR1);
    // write to CR1 to clear STOPF
    pI2Cx->CR1 |= 0x0;
}


/**
*   @brief check if data register is empty
*   @param param1 pointer of I2C base register  
*   @return flag status 1 or 0
*/
uint8_t I2C_SlaveShouldWrite(I2C_RegDef_t *pI2Cx){
    return I2C_StatusFlag(pI2Cx->SR1,I2C_FLAG_TXE);
}

/**
*   @brief slave writes data to data register
*   @param param1 pointer of I2C base register  
*   @param param2 data byte to be written
*   @return None
*/
void I2C_SlaveWriteData(I2C_RegDef_t *pI2Cx,uint8_t data){
    pI2Cx->DR = data;
}

/**
*   @brief check if NACK is detected by slave
*   @param param1 pointer of I2C base register  
*   @return flag status 1 or 0
*/
uint8_t I2C_SlaveEndWriteDetected(I2C_RegDef_t *pI2Cx){
    return I2C_StatusFlag(pI2Cx->SR1,I2C_FLAG_AF);
}

/**
*   @brief slave clear NACK flag
*   @param param1 pointer of I2C base register
*   @return None
*/
void I2C_SlaveEndWrite(I2C_RegDef_t *pI2Cx){
    pI2Cx->SR1 &= ~(1<<I2C_AF);
}



/**
* @brief This function helps master to send data with Interrupt
* @param param1 pointer of I2C Handle
* @param param2 pointer of Tx buffer
* @param param3 length of data
* @param param4 slave address
* @return communication state
*/
uint8_t I2C_MasterSendDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t repeatedStart){

    if(pI2CHandle->TxRxState != I2C_STATE_READY){
        return pI2CHandle->TxRxState; // return busy state
    }

    pI2CHandle->pTxBuffer = pTxBuffer;
    pI2CHandle->TxLen = Len;
    pI2CHandle->DevAddr = SlaveAddr;
    pI2CHandle->sr = repeatedStart;
    pI2CHandle->TxRxState = I2C_STATE_BUSY_IN_TX;

    

    // generate start condition
    generate_start_condition(pI2CHandle->pI2Cx);

    I2C_ManageInterrupt(pI2CHandle,ENABLE);

    return I2C_STATE_READY;
}

/**
* @brief This function helps master to receive data with Interrupt
* @param param1 pointer of I2C Handle
* @param param2 pointer of Rx buffer
* @param param3 length of data
* @param param4 slave address
* @return communication state
*/
uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t repeatedStart){
    
    if(pI2CHandle->TxRxState != I2C_STATE_READY){
        return pI2CHandle->TxRxState; // return busy state
    }

    pI2CHandle->pRxBuffer = pRxBuffer;
    pI2CHandle->RxLen = Len;
    pI2CHandle->DevAddr = SlaveAddr;
    pI2CHandle->sr = repeatedStart;
    pI2CHandle->TxRxState = I2C_STATE_BUSY_IN_RX;

    // generate start condition
    generate_start_condition(pI2CHandle->pI2Cx);

    I2C_ManageInterrupt(pI2CHandle,ENABLE);

    return I2C_STATE_READY;
}

/*
* @brief This function is called when transmission is complete in interrupt mode
* @param param1 pointer of I2C Handle
* @param param2 status of transmission
* @return None
*/
static void I2C_SendCallBack(I2C_Handle_t *pI2CHandle, uint8_t status){
    // clear state and member elements of handle structure
    
    I2C_ManageInterrupt(pI2CHandle,DISABLE);
    
    pI2CHandle->pTxBuffer = NULL;
    pI2CHandle->TxLen = 0;
    pI2CHandle->pRxBuffer = NULL;
    pI2CHandle->RxLen = 0;
    pI2CHandle->DevAddr = 0;
    pI2CHandle->TxRxState = I2C_STATE_READY;
    
    // notify application about reception complete
    I2C_AppCallback(pI2CHandle,status);
}

/*
* @brief This function handles I2C event interrupts
* @param param1 pointer of I2C Handle
* @return None
*/
void I2C_EV_IRQHandling(I2C_Handle_t *pI2CHandle){
    uint8_t temp1, temp2, temp3;
    // checking ITEVTEN bit
    temp1 = (pI2CHandle->pI2Cx->CR2 >> I2C_ITEVTEN) & 0x1;
    // checking ITBUFEN bit
    temp2 = (pI2CHandle->pI2Cx->CR2 >> I2C_ITBUFEN) & 0x1;

    // checking SB flag
    temp3 = (pI2CHandle->pI2Cx->SR1 >> I2C_SB) & 0x1;
    if(temp1 && temp3){
        // SB flag is set
        read_status_register(pI2CHandle->pI2Cx,I2C_READ_SR1);
        if(pI2CHandle->TxRxState == I2C_STATE_BUSY_IN_TX)
            send_slave_address_with_rw_bit(pI2CHandle->pI2Cx,pI2CHandle->DevAddr,I2C_MASTER_WRITE);
        else if(pI2CHandle->TxRxState == I2C_STATE_BUSY_IN_RX)
            send_slave_address_with_rw_bit(pI2CHandle->pI2Cx,pI2CHandle->DevAddr,I2C_MASTER_READ);
    }

    // checking ADDR flag
    temp3 = (pI2CHandle->pI2Cx->SR1 >> I2C_ADDR) & 0x1;
    if(temp1 && temp3){
        // ADDR flag is set
        uint8_t mode = (pI2CHandle->pI2Cx->SR2 >> I2C_MSL) & 0x1;
        if(mode == I2C_MODE_MASTER){
            // master mode
            if(pI2CHandle->TxRxState == I2C_STATE_BUSY_IN_TX){
                // if transmitting, clear ADDR flag
                read_status_register(pI2CHandle->pI2Cx,I2C_READ_SR1);
                read_status_register(pI2CHandle->pI2Cx,I2C_READ_SR2);
            }else if(pI2CHandle->TxRxState == I2C_STATE_BUSY_IN_RX){
                // if receiving
                if(pI2CHandle->RxLen == 1 && !pI2CHandle->sr){
                    // disable ack
                    I2C_ChangeAck(pI2CHandle->pI2Cx,I2C_ACK_DISABLE);
                    // clear ADDR flag
                    read_status_register(pI2CHandle->pI2Cx,I2C_READ_SR1);
                    read_status_register(pI2CHandle->pI2Cx,I2C_READ_SR2);
                    // generate stop condition
                    generate_stop_condition(pI2CHandle->pI2Cx);
                }else{
                    // clear ADDR flag
                    read_status_register(pI2CHandle->pI2Cx,I2C_READ_SR1);
                    read_status_register(pI2CHandle->pI2Cx,I2C_READ_SR2);
                }
            }
        }else{
            // slave mode
            read_status_register(pI2CHandle->pI2Cx,I2C_READ_SR1);
            read_status_register(pI2CHandle->pI2Cx,I2C_READ_SR2);
            /* Optimization for Speed */
            // uint8_t communication_state = (pI2CHandle->pI2Cx->SR2 >> I2C_TRA) & 0x1;
            // if(communication_state == I2C_STATE_BUSY_IN_TX){
            //     // slave transmitting data
            //     I2C_AppCallback(pI2CHandle,I2C_EV_DATA_REQ);
            // }
        }
    }

    // checking stopf flag
    temp3 = (pI2CHandle->pI2Cx->SR1 >> I2C_STOPF) & 0x1;
    if(temp1 && temp3){
        // STOPF flag is set
        uint8_t mode = (pI2CHandle->pI2Cx->SR2 >> I2C_MSL) & 0x1;
        uint8_t communication_state = (pI2CHandle->pI2Cx->SR2 >> I2C_TRA) & 0x1;
        if(mode == I2C_MODE_SLAVE && communication_state == I2C_STATE_BUSY_IN_RX){
            // clear STOPF flag
            read_status_register(pI2CHandle->pI2Cx,I2C_READ_SR1);
            // write to CR1 to clear STOPF
            pI2CHandle->pI2Cx->CR1 |= 0x0;
            
            // reset all member elements of handle structure and send callback to application
            I2C_AppCallback(pI2CHandle,I2C_EV_STOP_DETECTED);
    
        }
        
    }

    // checking RxNE flag
    temp3 = (pI2CHandle->pI2Cx->SR1 >> I2C_RXNE) & 0x1;
    if(temp1 && temp2 && temp3){
        // RxNE flag is set
        uint8_t mode = (pI2CHandle->pI2Cx->SR2 >> I2C_MSL) & 0x1;
        if(mode == I2C_MODE_MASTER){
            // master mode
            if(pI2CHandle->TxRxState == I2C_STATE_BUSY_IN_RX && pI2CHandle->RxLen > 0){
                if(pI2CHandle->RxLen == 2 && !pI2CHandle->sr){
                    I2C_ChangeAck(pI2CHandle->pI2Cx,I2C_ACK_DISABLE);
                    generate_stop_condition(pI2CHandle->pI2Cx);
                }
                *(pI2CHandle->pRxBuffer) = pI2CHandle->pI2Cx->DR;
                pI2CHandle->RxLen--;
                pI2CHandle->pRxBuffer++;
                if(pI2CHandle->RxLen == 0){
                    // reset all member elements of handle structure and send callback to application
                    I2C_SendCallBack(pI2CHandle,I2C_EV_RX_COMPLETE);
                    if(pI2CHandle->i2cConfig.I2C_ACKControl == I2C_ACK_ENABLE){
                        I2C_ChangeAck(pI2CHandle->pI2Cx,I2C_ACK_ENABLE);
                    }
                }
            }
        }else{
            // slave mode
            uint8_t communication_state = (pI2CHandle->pI2Cx->SR2 >> I2C_TRA) & 0x1;
            if(communication_state == I2C_STATE_BUSY_IN_RX){
                // slave receiving data
                I2C_AppCallback(pI2CHandle,I2C_EV_DATA_RCV);
            }
        }
    }
    
    // checking TxE flag
    temp3 = (pI2CHandle->pI2Cx->SR1 >> I2C_TXE) & 0x1;
    if(temp1 && temp2 && temp3){
        // TXE flag is set
        uint8_t mode = (pI2CHandle->pI2Cx->SR2 >> I2C_MSL) & 0x1;
        if(mode == I2C_MODE_MASTER){
            if(pI2CHandle->TxRxState == I2C_STATE_BUSY_IN_TX && pI2CHandle->TxLen > 0){
                pI2CHandle->pI2Cx->DR = *(pI2CHandle->pTxBuffer); // write data to data register
                pI2CHandle->pTxBuffer++;
                pI2CHandle->TxLen--;
            }
        }else{
            // slave mode
            uint8_t communication_state = (pI2CHandle->pI2Cx->SR2 >> I2C_TRA) & 0x1;
            if(communication_state == I2C_STATE_BUSY_IN_TX){
                // slave transmitting data
                I2C_AppCallback(pI2CHandle,I2C_EV_DATA_REQ);
            }
        }
    }

    // checking BTF flag
    temp3 = (pI2CHandle->pI2Cx->SR1 >> I2C_BTF) & 0x1;
    if(temp1 && temp3){
        // BTF flag is set
        uint8_t mode = (pI2CHandle->pI2Cx->SR2 >> I2C_MSL) & 0x1;
        uint8_t txe = (pI2CHandle->pI2Cx->SR1 >> I2C_TXE) & 0x1; // check TXE flag
        if(mode == I2C_MODE_MASTER){
            // master mode
            if(txe && pI2CHandle->TxLen == 0 && pI2CHandle->TxRxState == I2C_STATE_BUSY_IN_TX){
                if(!pI2CHandle->sr){
                    generate_stop_condition(pI2CHandle->pI2Cx);
                }
                // reset all member elements of handle structure and send callback to application and disable interrupts
                I2C_SendCallBack(pI2CHandle,I2C_EV_TX_COMPLETE);
            }
        }
    }
}

/*
* @brief This function handles I2C error interrupts
* @param param1 pointer of I2C Handle
* @return None
*/
void I2C_ER_IRQHandling(I2C_Handle_t *pI2CHandle){
    uint8_t temp1, temp2;
    // checking ITERREN bit
    temp1 = (pI2CHandle->pI2Cx->CR2 >> I2C_ITERREN) & 0x1;

    // checking BERR flag
    temp2 = (pI2CHandle->pI2Cx->SR1 >> I2C_TXE) & 0x1;
    if(temp1 && temp2){
        // BERR flag is set
        pI2CHandle->pI2Cx->SR1 &= ~(1<<I2C_BERR); // clear BERR flag
        // notify application about error
        I2C_AppCallback(pI2CHandle,I2C_ERROR_BERR);
    }

    // checking ARLO flag
    temp2 = (pI2CHandle->pI2Cx->SR1 >> I2C_ARLO) & 0x1;
    if(temp1 && temp2){
        // ARLO flag is set
        pI2CHandle->pI2Cx->SR1 &= ~(1<<I2C_ARLO); // clear ARLO flag
        // notify application about error
        I2C_AppCallback(pI2CHandle,I2C_ERROR_ARLO);
    }

    // checking AF flag
    temp2 = (pI2CHandle->pI2Cx->SR1 >> I2C_AF) & 0x1;
    if(temp1 && temp2){
        // AF flag is set
        uint8_t mode = (pI2CHandle->pI2Cx->SR2 >> I2C_MSL) & 0x1;
        //clear AF flag
        pI2CHandle->pI2Cx->SR1 &= ~(1<<I2C_AF);
        if(mode == I2C_MODE_SLAVE){
            // slave mode
            I2C_AppCallback(pI2CHandle,I2C_ERROR_AF);
            
        }else{
            // master mode
            generate_stop_condition(pI2CHandle->pI2Cx);
            // notify application about error
            I2C_SendCallBack(pI2CHandle,I2C_ERROR_AF);
            
        }
    }

    // checking OVR flag
    temp2 = (pI2CHandle->pI2Cx->SR1 >> I2C_OVR) & 0x1;
    if(temp1 && temp2){
        // OVR flag is set
        //clear OVR flag
        pI2CHandle->pI2Cx->SR1 &= ~(1<<I2C_OVR);
        // notify application about error
        I2C_AppCallback(pI2CHandle,I2C_ERROR_OVR);  
    }

    // checking TIMEOUT flag
    temp2 = (pI2CHandle->pI2Cx->SR1 >> I2C_TIMEOUT) & 0x1;
    if(temp1 && temp2){
        // TIMEOUT flag is set
        // clear TIMEOUT flag
        pI2CHandle->pI2Cx->SR1 &= ~(1<<I2C_TIMEOUT);
        // notify application about error
        I2C_AppCallback(pI2CHandle,I2C_ERROR_TIMEOUT);
    }

}