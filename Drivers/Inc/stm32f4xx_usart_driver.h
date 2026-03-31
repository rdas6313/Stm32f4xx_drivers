#include "cstm32f407xx.h"

/* USART Configuration structure */
typedef struct
{
    uint8_t Mode;                // tranmit or receive or both
    uint32_t Baud;               // Baudrate
    uint8_t NoOfStopBit;         // no of stop bits
    uint8_t WordLength;          // world length
    uint8_t ParityControl;       // Parity control
    uint8_t HardwareFlowControl; // hardware flow control
} USART_Config_t;

/* USART Handle structure */
typedef struct
{
    USART_Config_t usartConfig; // configuration
    USART_RegDef_t *pUSARTx;    // USART Base register address
    uint8_t *pTxBuffer;         // Transmit buffer address
    uint32_t lenTx;             // Transmit buffer length
    uint8_t *pRxBuffer;         // Received buffer address
    uint32_t lenRx;             // Received buffer length
    uint8_t statusTx;           // Transmit status
    uint8_t statusRx;           // Receive status

} USART_Handle_t;

/* USART Mode */
#define USART_MODE_TX 3    // Transmitter
#define USART_MODE_RX 2    // Receiver
#define USART_MODE_TXRX 12 // Transmit and Receive

/* USART Baudrate */
#define USART_STD_BAUD_1200 1200
#define USART_STD_BAUD_2400 400
#define USART_STD_BAUD_9600 9600
#define USART_STD_BAUD_19200 19200
#define USART_STD_BAUD_38400 38400
#define USART_STD_BAUD_57600 57600
#define USART_STD_BAUD_115200 115200
#define USART_STD_BAUD_230400 230400
#define USART_STD_BAUD_460800 460800
#define USART_STD_BAUD_921600 921600
#define USART_STD_BAUD_2M 2000000
#define SUART_STD_BAUD_3M 3000000

/* USART Stop bit */
#define USART_STOP_BIT_1 0   // 1 stop bit
#define USART_STOP_BIT_0_5 1 // 0.5 stop bit
#define USART_STOP_BIT_2 2   // 2 stop bit
#define USART_STOP_BIT_1_5 3 // 1.5 stop bit

/* USART Word Length */
#define USART_WORD_LENGTH_8 0 // 8 data bits
#define USART_WORD_LENGTH_9 1 // 9 data bits

/* USART Parity*/
#define USART_PARITY_DISABLE 0     // disable parity
#define USART_PARITY_ENABLE_ODD 1  // enable odd parity
#define USART_PARITY_ENABLE_EVEN 2 // enable even parity

/* Hardware flow control */
#define USART_HW_FLOW_CONTROL_DISABLE 0 // Hardware flow control disable
#define USART_HW_FLOW_CONTROL_CTS 1     // Hardware flow control CTS enable
#define USART_HW_FLOW_CONTROL_RTS 2     // Hardware flow control RTS enable
#define USART_HW_FLOW_CONTROL_CTS_RTS 3 // Hardware flow control CTS and RTS enable

/* USART status flags*/
#define STATUS_FLAG_PARITY_ERROR 0
#define STATUS_FLAG_FRAME_ERROR 1
#define STATUS_FLAG_NOISE_DETECTED 2
#define STATUS_FLAG_OVERRUN_ERROR 3
#define STATUS_FLAG_IDLE 4
#define STATUS_FLAG_RX_NOT_EMPTY 5
#define STATUS_FLAG_TRANSMISSION_COMPLETE 6
#define STATUS_FLAG_TX_EMPTY 7
#define STATUS_FLAG_LIN_BREAK 8
#define STATUS_FLAG_CTS 9

/* USART control status*/
#define IS_9_DATA_BITS_WORD(CR1) (CR1 & (1 << 12))
#define IS_PARITY_ENABLE(CR1) (CR1 & (1 << 10))
#define IS_ODD_PARITY(CR1) (CR1 & (1 << 9))

/* USART OPERATION STATUS */
#define USART_STATUS_BUSY 1
#define USART_STATUS_READY 0

/* USART CALLBACKS */
#define USART_CALLBACK_DATA_SEND_COMPLETED 1
#define USART_CALLBACK_DATA_SEND_ERROR 2
#define USART_CALLBACK_TRANMISSION_COMPLETED 3
#define USART_CALLBACK_DATA_RECV_ERROR 4
#define USART_CALLBACK_DATA_RECV_COMPLETED 5
#define USART_CALLBACK_ORE 6
#define USART_CALLBACK_CTS 7
#define USART_CALLBACK_IDLE 8
#define USART_CALLBACK_PARITY_ERROR 9

/* USART IRQs */
#define USART3_IRQ 39
#define USART1_IRQ 37

/*
 * Peripheral Clock setup
 */
void USART_PeriClockControl(USART_RegDef_t *pUSARTx, uint8_t EnorDi);

/*
 * Init and De-init
 */
void USART_Init(USART_Handle_t *pUSARTHandle);
void USART_DeInit(USART_RegDef_t *pUSARTx);

/*
 * Data Send and Receive
 */
void USART_SendData(USART_RegDef_t *pUSARTx, uint8_t *pTxBuffer, uint32_t Len);
void USART_ReceiveData(USART_RegDef_t *pUSARTx, uint8_t *pRxBuffer, uint32_t Len);
uint8_t USART_SendDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len);
uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len);

/*
 * IRQ Configuration and ISR handling
 */
void USART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void USART_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void USART_IRQHandling(USART_Handle_t *pHandle);

/*
 * Other Peripheral Control APIs
 */
void USART_PeripheralControl(USART_RegDef_t *pUSARTx, uint8_t EnOrDi);
uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx, uint32_t FlagName);
void USART_ClearFlag(USART_RegDef_t *pUSARTx, uint16_t StatusFlagName);

/*
 * Application callback
 */
void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle, uint8_t AppEv);
