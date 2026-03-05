#ifndef INC_STM32F4XX_GPIO_DRIVER_H
#define INC_STM32F4XX_GPIO_DRIVER_H



#include "stm32f407xx.h"

typedef struct
{
    uint8_t GPIO_PinNumber;
    uint8_t GPIO_PinMode;
    uint8_t GPIO_PinSpeed;
    uint8_t GPIO_PinPuPdControl;
    uint8_t GPIO_PinOPType;
    uint8_t GPIO_PinAltFunMode;
} GPIO_PinConfig_t;

typedef struct
{
    GPIO_RegDef_t *pGPIOx;           // Holds the base address of GPIO port to which this pin belongs
    GPIO_PinConfig_t GPIO_PinConfig; // Holds GPIO Pin configurations settings

} GPIO_Handle_t;

// GPIO API prototypes

// GPIO Clock enable and disable

void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnOrDi);

// GPIO Init and De-init

void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);

// Data read and write

uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber, uint8_t value);
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t value);
void GPIO_ToggleOuputPin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber);

// IRQ Configuration and ISR handling

void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnOrDi);
void GPIO_IRQHandling(uint8_t pinNumber);

// GPIO port possible modes
#define GPIO_MODE_IN 0
#define GPIO_MODE_OUT 1
#define GPIO_MODE_ALTFN 2
#define GPIO_MODE_ANALOG 3
#define GPIO_MODE_INT_RISE 4
#define GPIO_MODE_INT_FALL 5
#define GPIO_MODE_INT_RISE_FALL 6

// GPIO port possible Output type
#define GPIO_OUT_PUSPLL 0 // push pull
#define GPIO_OUT_OD 1     // open drain

// GPIO port output speed
#define GPIO_OUT_SPEED_LOW 0
#define GPIO_OUT_SPEED_MEDIUM 1
#define GPIO_OUT_SPEED_HIGH 2
#define GPIO_OUT_SPEED_VERY_HIGH 3

// GPIO port pull-up/pull-down
#define GPIO_PUPDR_NO 0  // No pull up, pull down
#define GPIO_PUPDR_PU 1  // pull up
#define GPIO_PUPDR_PDR 2 // pull down

// GPIO port possible alternate functions
#define GPIO_AF_0 0
#define GPIO_AF_1 1
#define GPIO_AF_2 2
#define GPIO_AF_3 3
#define GPIO_AF_4 4
#define GPIO_AF_5 5
#define GPIO_AF_6 6
#define GPIO_AF_7 7
#define GPIO_AF_8 8
#define GPIO_AF_9 9
#define GPIO_AF_10 10
#define GPIO_AF_11 11
#define GPIO_AF_12 12
#define GPIO_AF_13 13
#define GPIO_AF_14 14
#define GPIO_AF_15 15

// GPIO generic
#define GPIO_MAX_MODE 7
#define GPIO_MAX_OP_TYPE 2
#define GPIO_MAX_OP_SPEED 4
#define GPIO_MAX_PUPDR 3
#define GPIO_MAX_ALTF 16

// Pin state
#define GPIO_PIN_SET 1
#define GPIO_PIN_RESET 0


#endif