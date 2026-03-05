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
    GPIO_Handle_t sgpioMaster;
    sgpioMaster.pGPIOx = GPIOA;
    sgpioMaster.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    sgpioMaster.GPIO_PinConfig.GPIO_PinAltFunMode = GPIO_AF_5;
    sgpioMaster.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OUT_SPEED_MEDIUM;
    sgpioMaster.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NO;
    sgpioMaster.GPIO_PinConfig.GPIO_PinOPType = GPIO_OUT_PUSPLL;

    GPIO_PeriClockControl(sgpioMaster.pGPIOx, ENABLE);

    // For Nss
    sgpioMaster.GPIO_PinConfig.GPIO_PinNumber = 4;
    GPIO_Init(&sgpioMaster);

    // For SCLK
    sgpioMaster.GPIO_PinConfig.GPIO_PinNumber = 5;
    GPIO_Init(&sgpioMaster);

    // // For MISO
    // sgpioMaster.GPIO_PinConfig.GPIO_PinNumber = 6;
    // GPIO_Init(&sgpioMaster);

    // For MOSI
    sgpioMaster.GPIO_PinConfig.GPIO_PinNumber = 7;
    GPIO_Init(&sgpioMaster);

    /*
     *   SPI2 GPIO
     *   NSS     ->  PB12
     *   SCK     ->  PB13
     *   MISO    ->  PB14
     *   MOSI    ->  PB15
     */

    GPIO_Handle_t sgpioSlave;
    sgpioSlave.pGPIOx = GPIOB;
    sgpioSlave.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    sgpioSlave.GPIO_PinConfig.GPIO_PinAltFunMode = GPIO_AF_5;
    sgpioSlave.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OUT_SPEED_MEDIUM;
    sgpioSlave.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NO;
    sgpioSlave.GPIO_PinConfig.GPIO_PinOPType = GPIO_OUT_PUSPLL;

    GPIO_PeriClockControl(sgpioSlave.pGPIOx, ENABLE);

    // For Nss
    sgpioSlave.GPIO_PinConfig.GPIO_PinNumber = 12;
    GPIO_Init(&sgpioSlave);

    // For SCLK
    sgpioSlave.GPIO_PinConfig.GPIO_PinNumber = 13;
    GPIO_Init(&sgpioSlave);

    // // For MISO
    // sgpioSlave.GPIO_PinConfig.GPIO_PinNumber = 14;
    // GPIO_Init(&sgpioSlave);

    // For MOSI
    sgpioSlave.GPIO_PinConfig.GPIO_PinNumber = 15;
    GPIO_Init(&sgpioSlave);

    /*
     *   Button Configuration
     */

    GPIO_Handle_t BtnHandle;
    BtnHandle.pGPIOx = GPIOA;
    BtnHandle.GPIO_PinConfig.GPIO_PinNumber = 0;
    BtnHandle.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
    BtnHandle.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OUT_SPEED_HIGH;
    BtnHandle.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NO;

    GPIO_PeriClockControl(BtnHandle.pGPIOx, ENABLE);
    GPIO_Init(&BtnHandle);
}

uint8_t send_data_len(SPI_Handle_t *spiMaster, SPI_Handle_t *spiSlave, uint8_t *data, uint8_t size)
{
    uint8_t rdata_len = 0;
    // slave enable
    SPI_EnableOrDisable(spiSlave->pSPIx, ENABLE);
    // master enable
    SPI_EnableOrDisable(spiMaster->pSPIx, ENABLE);

    // sending data byte length first from master
    SPI_Send(spiMaster->pSPIx, data, size);

    while (SPI_StatusFlag(spiMaster->pSPIx, BUSY_STATUS))
        ;

    // slave recv data byte length first from master
    SPI_Receive(spiSlave->pSPIx, &rdata_len, sizeof(rdata_len));

    // master disable
    SPI_EnableOrDisable(spiMaster->pSPIx, DISABLE);

    // slave disable
    SPI_EnableOrDisable(spiSlave->pSPIx, DISABLE);

    return rdata_len;
}

void send_data(SPI_Handle_t *spiMaster, SPI_Handle_t *spiSlave, uint8_t *data, uint8_t data_len, uint8_t *data_buf, uint8_t recv_data_len)
{
    // slave enable
    SPI_EnableOrDisable(spiSlave->pSPIx, ENABLE);
    // master enable
    SPI_EnableOrDisable(spiMaster->pSPIx, ENABLE);

    uint8_t i = 0;

    while (i < data_len)
    {

        // sending data byte length first from master
        SPI_Send(spiMaster->pSPIx, data + i, 1);

        while (SPI_StatusFlag(spiMaster->pSPIx, BUSY_STATUS))
            ;

        // slave recv data byte length first from master
        SPI_Receive(spiSlave->pSPIx, data_buf + i, 1);

        i++;
    }

    // master disable
    SPI_EnableOrDisable(spiMaster->pSPIx, DISABLE);

    // slave disable
    SPI_EnableOrDisable(spiSlave->pSPIx, DISABLE);
}

void SPI1_Inits()
{

    SPI_Handle_t spiMaster;
    spiMaster.pSPIx = SPI1;
    spiMaster.spiConfig.SPI_BusConfig = SPI_BUS_FULL_DUPLEX;
    spiMaster.spiConfig.SPI_CPHA = SPI_PHASE_1st_EDGE_SAMPLING;
    spiMaster.spiConfig.SPI_CPOL = SPI_POL_LOW_IDLE;
    spiMaster.spiConfig.SPI_DataFormat = SPI_DATA_FORMAT_8_BIT;
    spiMaster.spiConfig.SPI_DeviceMode = SPI_MODE_MASTER;
    spiMaster.spiConfig.SPI_Speed = SPI_CLK_DIV_8;
    spiMaster.spiConfig.SPI_SSM = SPI_SOFTWARE_SLAVE_DI;
    spiMaster.spiConfig.SPI_SSOE = SSOE_EN;

    SPI_PeriClockControl(spiMaster.pSPIx, ENABLE);
    SPI_Init(&spiMaster);

    SPI_Handle_t spiSlave;
    spiSlave.pSPIx = SPI2;
    spiSlave.spiConfig.SPI_BusConfig = SPI_BUS_SIMPLEX_RECEIVE;
    spiSlave.spiConfig.SPI_CPHA = SPI_PHASE_1st_EDGE_SAMPLING;
    spiSlave.spiConfig.SPI_CPOL = SPI_POL_LOW_IDLE;
    spiSlave.spiConfig.SPI_DataFormat = SPI_DATA_FORMAT_8_BIT;
    spiSlave.spiConfig.SPI_DeviceMode = SPI_MODE_SLAVE;
    spiSlave.spiConfig.SPI_Speed = SPI_CLK_DIV_8;
    spiSlave.spiConfig.SPI_SSM = SPI_SOFTWARE_SLAVE_DI;
    spiSlave.spiConfig.SPI_SSOE = SSOE_DI;

    SPI_PeriClockControl(spiSlave.pSPIx, ENABLE);
    SPI_Init(&spiSlave);

    while (1)
    {
        uint8_t data[] = "Hi Raja!";
        uint8_t data_len = strlen((char *)data) + 1;
        uint8_t data_buf[data_len + 1];
        strcpy((char *)data_buf, "");

        // uint8_t recv_data[100];
        uint8_t recv_data_len = 0;

        while (GPIO_ReadFromInputPin(GPIOA, 0) == LOW)
            ;

        delay();

        /*Sending data length first*/
        recv_data_len = send_data_len(&spiMaster, &spiSlave, &data_len, sizeof(data_len));

        /*Sending data*/
        send_data(&spiMaster, &spiSlave, data, data_len, data_buf, recv_data_len);

        for (int i = 0; i < 10; i++)
            ;
    }
}

int main()
{
    GPIO_Inits();
    SPI1_Inits();
    while (1)
        ;
    return 0;
}