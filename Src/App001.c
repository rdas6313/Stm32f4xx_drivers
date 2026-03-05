#include "stm32f4xx_gpio_driver.h"
#define HIGH 1
#define LOW 0

void delay()
{
    for (volatile int i = 0; i < 1000000; i++)
        ;
}

int main()
{
    GPIO_Handle_t ledHandle;
    ledHandle.pGPIOx = GPIOD;
    ledHandle.GPIO_PinConfig.GPIO_PinNumber = 13;
    ledHandle.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    ledHandle.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OUT_SPEED_LOW;
    ledHandle.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NO;
    ledHandle.GPIO_PinConfig.GPIO_PinOPType = GPIO_OUT_PUSPLL;
    ledHandle.GPIO_PinConfig.GPIO_PinAltFunMode = GPIO_AF_0;

    GPIO_PeriClockControl(ledHandle.pGPIOx, ENABLE);

    GPIO_Init(&ledHandle);

    GPIO_Handle_t BtnHandle;
    BtnHandle.pGPIOx = GPIOA;
    BtnHandle.GPIO_PinConfig.GPIO_PinNumber = 0;
    BtnHandle.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
    BtnHandle.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OUT_SPEED_HIGH;
    BtnHandle.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NO;

    GPIO_PeriClockControl(BtnHandle.pGPIOx, ENABLE);
    GPIO_Init(&BtnHandle);

    while (1)
    {
        if (GPIO_ReadFromInputPin(BtnHandle.pGPIOx, BtnHandle.GPIO_PinConfig.GPIO_PinNumber) == HIGH)
        {
            delay();
            GPIO_ToggleOuputPin(ledHandle.pGPIOx, ledHandle.GPIO_PinConfig.GPIO_PinNumber);
        }
    }
    return 0;
}