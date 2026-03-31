#ifndef INC_STM32F4XX_I2C_DRIVER_H_
#define INC_STM32F4XX_I2C_DRIVER_H_
#include "cstm32f407xx.h"

/* I2C configuration structure */

typedef struct
{
    uint32_t I2C_SCLSpeed;
    uint8_t I2C_DeviceAddress;
    uint8_t I2C_ACKControl;
    uint16_t I2C_FMDutyCycle;
} I2C_Config_t;

/* I2C Handle structure */
typedef struct
{
    I2C_RegDef_t *pI2Cx;    // I2C Base Register address
    I2C_Config_t i2cConfig; // Configurations
    uint8_t *pTxBuffer;     // Tx buffer address
    uint32_t TxLen;         // Tx length
    uint8_t *pRxBuffer;     // Rx buffer address
    uint32_t RxLen;         // Rx length
    uint8_t TxRxState;      // communication state
    uint8_t DevAddr;        // Device address
    uint8_t sr;           // repeated start value
} I2C_Handle_t;

/* I2C SCLSpeed */
#define I2C_SCL_SPEED_SM 100000
#define I2C_SCL_SPEED_FM 400000

/* I2C Ack control */
#define I2C_ACK_ENABLE 1
#define I2C_ACK_DISABLE 0

/* I2C DutyCycle */
#define I2C_FM_DUTY_2 0
#define I2C_FM_DUTY_16_by_9 1

/* I2C Status1 Flags */
#define I2C_FLAG_SB  (1 << 0)
#define I2C_FLAG_ADDR (1 << 1)  
#define I2C_FLAG_BTF  (1 << 2)
#define I2C_FLAG_STOPF (1 << 4)
#define I2C_FLAG_RXNE  (1 << 6)
#define I2C_FLAG_TXE  (1 << 7)
#define I2C_FLAG_ARLO  (1 << 9)
#define I2C_FLAG_AF   (1 << 10)
#define I2C_FLAG_OVR  (1 << 11)
#define I2C_FLAG_TIMEOUT (1 << 14)

/* I2C Status2 Flags */
#define I2C_FLAG_MSL  (1 << 0)
#define I2C_FLAG_BUSY (1 << 1)

// read status register flags
#define I2C_READ_SR1 1
#define I2C_READ_SR2 2

// r_w bit
#define I2C_MASTER_WRITE 0
#define I2C_MASTER_READ 1

// control register1 macros
#define I2C_PE 0
#define I2C_START 8
#define I2C_STOP 9
#define I2C_ACK 10

// control register2 macros
#define I2C_ITERREN 8
#define I2C_ITEVTEN 9
#define I2C_ITBUFEN 10

// status register1 macros
#define I2C_SB 0
#define I2C_ADDR 1
#define I2C_BTF 2
#define I2C_STOPF 4
#define I2C_RXNE 6
#define I2C_TXE 7
#define I2C_ARLO 9
#define I2C_AF 10
#define I2C_OVR 11
#define I2C_TIMEOUT 14
#define I2C_BERR 8
#define I2C_ARLO 9
#define I2C_AF 10
#define I2C_OVR 11
#define I2C_TIMEOUT 14

// status register2 macros
#define I2C_MSL 0
#define I2C_BUSY 1
#define I2C_TRA 2

// I2C communication state
#define I2C_STATE_READY 2
#define I2C_STATE_BUSY_IN_RX 0
#define I2C_STATE_BUSY_IN_TX 1

// I2C IRQ numbers
#define I2C1_EV_IRQ 31
#define I2C1_ER_IRQ 32
#define I2C2_EV_IRQ 33
#define I2C2_ER_IRQ 34
#define I2C3_EV_IRQ 72
#define I2C3_ER_IRQ 73

// I2C CALLBACK EVENTS
#define I2C_EV_TX_COMPLETE 0
#define I2C_EV_RX_COMPLETE 1
#define I2C_EV_STOP_DETECTED 2
#define I2C_ERROR_BERR 3
#define I2C_ERROR_ARLO 4
#define I2C_ERROR_AF 5
#define I2C_ERROR_OVR 6
#define I2C_ERROR_TIMEOUT 7
#define I2C_EV_DATA_REQ 8
#define I2C_EV_DATA_RCV 9


// I2C MODE Slave/Master
#define I2C_MODE_SLAVE 0
#define I2C_MODE_MASTER 1

// Enable or Disable repeated start
#define I2C_ENABLE_SR 1
#define I2C_DISABLE_SR 0

/* ************* */
/*    I2C API    */
/* ************* */

// I2C Clock enable and disable
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi);

// I2C Init and De-init
void I2C_Init(I2C_Handle_t *pI2CHandle);
void I2C_DeInit(I2C_RegDef_t *pI2Cx);

// IRQ Configuration and ISR handling
void I2C_IRQConfig(I2C_RegDef_t *pI2Cx,uint8_t IRQNumber, uint8_t EnOrDi);
void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);

// access status
uint8_t I2C_StatusFlag(uint32_t reg, uint16_t status_flag);

// Peripheral control
void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi);

// Application callback function
void I2C_AppCallback(I2C_Handle_t *pHandle, uint8_t status);

// Master sends data
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t repeatedStart);

// Master receives data
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t repeatedStart);

// Master sends data with Interrupt
uint8_t I2C_MasterSendDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t repeatedStart);

// Master receives data with Interrupt
uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t repeatedStart);

// IRQ Handler for event
void I2C_EV_IRQHandling(I2C_Handle_t *pI2CHandle);

// IRQ Handler for error
void I2C_ER_IRQHandling(I2C_Handle_t *pI2CHandle);



// Change ACKing
void I2C_ChangeAck(I2C_RegDef_t *pI2Cx,uint8_t EnOrDi);

// wait for slave select
void I2C_WaitForSlaveSelect(I2C_Handle_t *pI2CHandle);

// check if slave data is available
uint8_t I2C_SlaveDataAvailableForRead(I2C_RegDef_t *pI2Cx);

// slave receives data
uint8_t I2C_SlaveReadData(I2C_RegDef_t *pI2Cx);

// slave stop condition detected
uint8_t I2C_SlaveEndReadDetected(I2C_RegDef_t *pI2Cx);

// slave releases connection
void I2C_SlaveEndRead(I2C_RegDef_t *pI2Cx);

// slave ready to transmit
uint8_t I2C_SlaveShouldWrite(I2C_RegDef_t *pI2Cx);

// slave writes data to data register
void I2C_SlaveWriteData(I2C_RegDef_t *pI2Cx,uint8_t data);

// slave NACK detected
uint8_t I2C_SlaveEndWriteDetected(I2C_RegDef_t *pI2Cx);

// slave clear NACK flag
void I2C_SlaveEndWrite(I2C_RegDef_t *pI2Cx);

// Enable or Disable I2C Interrupt signals
void I2C_ManageInterrupt(I2C_Handle_t *pI2CHandle,uint8_t EnOrDi);


//uint8_t I2C_StatusFlag(uint32_t reg, uint16_t status_flag);

// start condition generation
void generate_start_condition(I2C_RegDef_t *pI2Cx);
// read status register flag
void read_status_register(I2C_RegDef_t *pI2Cx,uint8_t flag_type);
// stop condition generation
void generate_stop_condition(I2C_RegDef_t *pI2Cx);

#define I2C_StatusFlag(reg,status_flag) (((reg) & (status_flag)) ? 1 : 0)


#endif /* INC_STM32F4XX_I2C_DRIVER_H_ */