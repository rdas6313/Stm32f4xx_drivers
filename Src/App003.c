#include "stm32f4xx_gpio_driver.h"
#include "stm32f4xx_spi_driver.h"
#include <string.h>

#define HIGH 1
#define LOW 0

void delay()
{
    for (volatile int i = 0; i < 1000000; i++)
        ;
}

void GPIO_Inits()
{
    GPIO_Handle_t sgpio;
    sgpio.pGPIOx = GPIOA;
    sgpio.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    sgpio.GPIO_PinConfig.GPIO_PinAltFunMode = GPIO_AF_5;
    sgpio.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OUT_SPEED_MEDIUM;
    sgpio.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NO;
    sgpio.GPIO_PinConfig.GPIO_PinOPType = GPIO_OUT_PUSPLL;

    GPIO_PeriClockControl(sgpio.pGPIOx, ENABLE);

    // For Nss
    // sgpio.GPIO_PinConfig.GPIO_PinNumber = 4;
    // GPIO_Init(&sgpio);

    // For SCLK
    sgpio.GPIO_PinConfig.GPIO_PinNumber = 5;
    GPIO_Init(&sgpio);

    // For MISO
    // sgpio.GPIO_PinConfig.GPIO_PinNumber = 6;
    // GPIO_Init(&sgpio);

    // For MOSI
    sgpio.GPIO_PinConfig.GPIO_PinNumber = 7;
    GPIO_Init(&sgpio);
}

void SPI1_Inits()
{
    SPI_Handle_t spiHandle;
    spiHandle.pSPIx = SPI1;
    spiHandle.spiConfig.SPI_BusConfig = SPI_BUS_FULL_DUPLEX;
    spiHandle.spiConfig.SPI_CPHA = SPI_PHASE_1st_EDGE_SAMPLING;
    spiHandle.spiConfig.SPI_CPOL = SPI_POL_LOW_IDLE;
    spiHandle.spiConfig.SPI_DataFormat = SPI_DATA_FORMAT_8_BIT;
    spiHandle.spiConfig.SPI_DeviceMode = SPI_MODE_MASTER;
    spiHandle.spiConfig.SPI_Speed = SPI_CLK_DIV_2;
    spiHandle.spiConfig.SPI_SSM = SPI_SOFTWARE_SLAVE_EN;
    spiHandle.spiConfig.SPI_SSI = SPI_SSI_HIGH;

    SPI_PeriClockControl(spiHandle.pSPIx, ENABLE);
    SPI_Init(&spiHandle);

    uint8_t data[] = "Hello! SPI Working!";

    SPI_EnableOrDisable(spiHandle.pSPIx, ENABLE);

    SPI_Send(spiHandle.pSPIx, data, 20);

    SPI_EnableOrDisable(spiHandle.pSPIx, DISABLE);
}

int main()
{
    GPIO_Inits();
    SPI1_Inits();
    while (1)
        ;
    return 0;
}