#include "stm32f407xx.h"

/* SPI Configuration structure */
typedef struct
{

    uint8_t SPI_DeviceMode; // master or slave
    uint8_t SPI_BusConfig;  // Full-duplex/half-duplex/simplex communication
    uint8_t SPI_DataFormat; // 8bit/16bit shift register
    uint8_t SPI_CPHA;       // Clock phase (1st edge or 2nd edge sampling)
    uint8_t SPI_CPOL;       // clock polarity (Clock idle high/low)
    uint8_t SPI_SSM;        // software/hardware slave management
    uint8_t SPI_SSI;        // internal slave select
    uint8_t SPI_Speed;      // serial clock speed
    uint8_t SPI_SSOE;       // SS output enable
} SPI_Config_t;

/* SPI Handle structure */
typedef struct
{
    SPI_RegDef_t *pSPIx;    // SPI Base Register address
    SPI_Config_t spiConfig; // Configurations
    uint8_t *pBuffer;       // Buffer for sender or receiver
    uint32_t len;           // len of buffer
    uint8_t status;         // Busy or Ready
} SPI_Handle_t;

/* ************* */
/*    SPI API    */
/* ************* */

// SPI Clock enable and disable

void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);

// SPI Init and De-init

void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_DeInit(SPI_RegDef_t *pSPIx);

// Data read and write
void SPI_Send(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t len);
void SPI_Receive(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t len);

// Data read and write with Interrupt
uint8_t SPI_SendWithIT(SPI_Handle_t *pSPIx, uint8_t *pTxBuffer, uint32_t len);
uint8_t SPI_ReceiveWithIT(SPI_Handle_t *pSPIx, uint8_t *pRxBuffer, uint32_t len);

// IRQ Configuration and ISR handling
void SPI_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnOrDi);
void SPI_IRQHandling(SPI_Handle_t *pHandle);

// Error Interrupt Enable or disable
void SPI_ErrorInt(SPI_RegDef_t *pSPIx, uint8_t EnorDi);

// Other SPI functions
void SPI_EnableOrDisable(SPI_RegDef_t *pSPIx, uint8_t EnorDi);

// access status
uint8_t SPI_StatusFlag(SPI_RegDef_t *pSPIx, uint16_t status_flag);

// Application callback function
void SPI_AppCallback(SPI_Handle_t *pHandle, uint8_t status);

// clear OVR error
void SPI_Clear_OVR_ERROR(SPI_Handle_t *pHandle);



/*
 * SPI MACROS
 */

// App callback status
#define CALLBACK_TX_COMPLETE 1
#define CALLBACK_RX_COMPLETE 2
#define CALLBACK_OVR_ERROR 3
#define CALLBACK_MODF_ERROR 4

// SPI IRQ
#define SPI1_IRQ 35
#define SPI2_IRQ 36

// SSOE
#define SSOE_EN 1
#define SSOE_DI 0

// SPI status flags
#define BUSY_STATUS 7
#define MODE_FAULT_STATUS 5
#define TRANSMIT_BUFFER_EMPTY_STATUS 1
#define RECEIVE_BUFFER_NOT_EMPTY_STATUS 0
#define SPI_DATAFRAME_FORMAT 11
#define READY_STATUS 255

// Device modes
#define SPI_MODE_MASTER 1
#define SPI_MODE_SLAVE 0

// Bus Configurations
#define SPI_BUS_FULL_DUPLEX 0
#define SPI_BUS_HALF_DUPLEX_RECEIVE 1  // ACT AS A RECEIVER
#define SPI_BUS_HALF_DUPLEX_TRANSMIT 2 // ACT AS A TRANSMITTER
#define SPI_BUS_SIMPLEX_RECEIVE 3      // ACT AS A RECEIVER

// Data formats
#define SPI_DATA_FORMAT_8_BIT 0
#define SPI_DATA_FORMAT_16_BIT 1

// CPHA
#define SPI_PHASE_1st_EDGE_SAMPLING 0
#define SPI_PHASE_2nd_EDGE_SAMPLING 1

// CPOL
#define SPI_POL_LOW_IDLE 0
#define SPI_POL_HIGH_IDLE 1

// Software slave management
#define SPI_SOFTWARE_SLAVE_EN 1
#define SPI_SOFTWARE_SLAVE_DI 0

#define SPI_SSI_HIGH 1
#define SPI_SSI_LOW 0

// Serial Clock speed
#define SPI_CLK_DIV_2 0
#define SPI_CLK_DIV_4 1
#define SPI_CLK_DIV_8 2
#define SPI_CLK_DIV_16 3
#define SPI_CLK_DIV_32 4
#define SPI_CLK_DIV_64 5
#define SPI_CLK_DIV_128 6
#define SPI_CLK_DIV_256 7

// SPI Status flags
#define SPI_Tx_EMPTY_FLAG (1 << 1)
#define SPI_Rx_FULL_FLAG (1 << 0)
#define SPI_DFF_FLAG (1 << 11)

// SPI Control register
#define SPI_TX_INT 7
#define SPI_RX_INT 6
#define SPI_ERR_INT 5

// SPI Error flags
#define SPI_OVR_ERR 6
#define SPI_MODF_ERR 5
#define SPI_CRCERR_ERR 4