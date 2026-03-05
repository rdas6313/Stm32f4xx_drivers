#include "stm32f4xx_usart_driver.h"
#include "stm32f4xx_gpio_driver.h"
#include <string.h>

#define LOW 0
#define HIGH 1

// USART1_TX -> PB6
// USART1_RX -> PB7
void GPIO_Inits()
{
    GPIO_Handle_t usartHandle;
    usartHandle.pGPIOx = GPIOB;
    usartHandle.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    usartHandle.GPIO_PinConfig.GPIO_PinAltFunMode = GPIO_AF_7;
    usartHandle.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OUT_SPEED_MEDIUM;
    usartHandle.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NO;
    usartHandle.GPIO_PinConfig.GPIO_PinOPType = GPIO_OUT_PUSPLL;

    // enable GPIO_Clock
    GPIO_PeriClockControl(usartHandle.pGPIOx, ENABLE);

    // PB6 -> USART1_TX
    usartHandle.GPIO_PinConfig.GPIO_PinNumber = 6;
    GPIO_Init(&usartHandle);

    // PB7 -> USART1_RX
    usartHandle.GPIO_PinConfig.GPIO_PinNumber = 7;
    GPIO_Init(&usartHandle);

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

void Usart_Init()
{
    USART_Handle_t usart1Handle;
    usart1Handle.usartConfig.Baud = USART_STD_BAUD_9600;
    usart1Handle.usartConfig.HardwareFlowControl = USART_HW_FLOW_CONTROL_DISABLE;
    usart1Handle.usartConfig.Mode = USART_MODE_TXRX;
    usart1Handle.usartConfig.NoOfStopBit = USART_STOP_BIT_1;
    usart1Handle.usartConfig.ParityControl = USART_PARITY_DISABLE;
    usart1Handle.usartConfig.WordLength = USART_WORD_LENGTH_8;

    usart1Handle.pUSARTx = USART1;

    // enable USART peripheral clock
    USART_PeriClockControl(usart1Handle.pUSARTx, ENABLE);

    // init usart1
    USART_Init(&usart1Handle);
}

void wait_For_Btn_Press()
{
    while (GPIO_ReadFromInputPin(GPIOA, 0) == LOW)
        ;
    for (volatile int i = 0; i < 1000000; i++)
        ;
}

void Send_Recv()
{
    char data[] = "Hi! USART Working!";
    char recv_data[100] = {0};
    uint16_t len = strlen(data) + 1;
    while (1)
    {
        wait_For_Btn_Press();
        USART_PeripheralControl(USART1, ENABLE);
        uint16_t i = 0;
        while (i < len)
        {
            USART_SendData(USART1, (uint8_t *)data + i, 1);
            USART_ReceiveData(USART1, (uint8_t *)recv_data + i, 1);
            i++;
        }
        USART_PeripheralControl(USART1, DISABLE);
    }
}

int main()
{
    GPIO_Inits();
    Usart_Init();
    Send_Recv();
    while (1)
        ;
    return 0;
}