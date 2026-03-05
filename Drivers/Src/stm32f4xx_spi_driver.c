#include "stm32f4xx_spi_driver.h"

// internal functions
static void SPI_send_data_handle(SPI_Handle_t *pHandle);
static void SPI_recv_data_handle(SPI_Handle_t *pHandle);
static void SPI_ovr_error_handle(SPI_Handle_t *pHandle);
static void SPI_modf_error(SPI_Handle_t *pHandle);

/**
 * @brief This function enable and disable SPI peripheral clock
 *
 * @param param1 Base address of SPI peripheral
 * @param param2 Enable or Disable value
 * @return None
 */

void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
    if (EnOrDi == ENABLE)
    {
        if (pSPIx == SPI1)
            SPI1_PCLK_EN();
        else if (pSPIx == SPI2)
            SPI2_PCLK_EN();
        else if (pSPIx == SPI3)
            SPI3_PCLK_EN();
    }
    else
    {
        if (pSPIx == SPI1)
            SPI1_PCLK_DI();
        else if (pSPIx == SPI2)
            SPI2_PCLK_DI();
        else if (pSPIx == SPI3)
            SPI3_PCLK_DI();
    }
}

/**
 * @brief This function Initialize SPI peripheral.
 *
 * @param param1 pointer of SPI_Handle_t struct
 * @return None
 */

void SPI_Init(SPI_Handle_t *pSPIHandle)
{
    // Mode Config
    pSPIHandle->pSPIx->CR1 &= ~(1 << 2);
    pSPIHandle->pSPIx->CR1 |= (pSPIHandle->spiConfig.SPI_DeviceMode << 2);

    // Bus Config
    if (pSPIHandle->spiConfig.SPI_BusConfig == SPI_BUS_FULL_DUPLEX)
    {
        pSPIHandle->pSPIx->CR1 &= ~(1 << 15);
        pSPIHandle->pSPIx->CR1 &= ~(1 << 10);
    }
    else if (pSPIHandle->spiConfig.SPI_BusConfig == SPI_BUS_HALF_DUPLEX_RECEIVE)
    {
        pSPIHandle->pSPIx->CR1 |= (1 << 15);
        pSPIHandle->pSPIx->CR1 &= ~(1 << 14);
    }
    else if (pSPIHandle->spiConfig.SPI_BusConfig == SPI_BUS_HALF_DUPLEX_TRANSMIT)
    {
        pSPIHandle->pSPIx->CR1 |= (1 << 15);
        pSPIHandle->pSPIx->CR1 |= (1 << 14);
    }
    else
    {
        pSPIHandle->pSPIx->CR1 &= ~(1 << 15);
        pSPIHandle->pSPIx->CR1 |= (1 << 10);
    }

    // Data format
    pSPIHandle->pSPIx->CR1 &= ~(1 << 11);
    pSPIHandle->pSPIx->CR1 |= (pSPIHandle->spiConfig.SPI_DataFormat << 11);

    // CPHA
    pSPIHandle->pSPIx->CR1 &= ~(1 << 0);
    pSPIHandle->pSPIx->CR1 |= (pSPIHandle->spiConfig.SPI_CPHA << 0);

    // CPOL
    pSPIHandle->pSPIx->CR1 &= ~(1 << 1);
    pSPIHandle->pSPIx->CR1 |= (pSPIHandle->spiConfig.SPI_CPOL << 1);

    // SSM
    pSPIHandle->pSPIx->CR1 &= ~(1 << 9);
    pSPIHandle->pSPIx->CR1 |= (pSPIHandle->spiConfig.SPI_SSM << 9);
    if (pSPIHandle->spiConfig.SPI_SSM == SPI_SOFTWARE_SLAVE_EN)
    {
        pSPIHandle->pSPIx->CR1 &= ~(1 << 8);
    }

    // SSI
    if (pSPIHandle->spiConfig.SPI_SSI == SPI_SSI_HIGH)
    {
        pSPIHandle->pSPIx->CR1 |= (1 << 8);
    }
    else
    {
        pSPIHandle->pSPIx->CR1 &= ~(1 << 8);
    }

    // SSOE
    if (pSPIHandle->spiConfig.SPI_SSM == SPI_SOFTWARE_SLAVE_DI && pSPIHandle->spiConfig.SPI_SSOE == SSOE_EN)
    {
        pSPIHandle->pSPIx->CR2 |= (1 << 2);
    }

    // Clock speed
    pSPIHandle->pSPIx->CR1 &= ~(0x7 << 3);
    pSPIHandle->pSPIx->CR1 |= (pSPIHandle->spiConfig.SPI_Speed << 3);
}

/**
 * @brief This function De-Initialize SPI peripheral.
 *
 * @param param1 Base address of SPI peripheral Register
 * @return None
 */

void SPI_DeInit(SPI_RegDef_t *pSPIx)
{
    if (pSPIx == SPI1)
    {
        RCC->APB2RSTR |= (1 << 12);
    }
    else if (pSPIx == SPI2)
    {
        RCC->APB1RSTR |= (1 << 14);
    }
    else if (pSPIx == SPI3)
    {
        RCC->APB1RSTR |= (1 << 15);
    }
}

/**
 * @brief This function get status of a flag.
 *
 * @param param1 Base address of a register
 * @param param2 16 bit flag
 * @return Non zero value represents 1 and zero represents 0
 */

uint8_t getFlag(uint32_t reg_val, uint16_t flag)
{
    return (reg_val & flag) > 0 ? 1 : 0;
}

/**
 * @brief This function Sends data to Receiver using SPI. This is a blocking function call.
 *
 * @param param1 Base address of SPI register
 * @param param2 Base address of Transmit buffer data
 * @param param3 length of Transmit buffer data
 * @return None
 */

void SPI_Send(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t len)
{
    while (len > 0)
    {
        while (getFlag(pSPIx->SR, SPI_Tx_EMPTY_FLAG) == RESET)
            ;

        if (getFlag(pSPIx->CR1, SPI_DFF_FLAG) == SET)
        {
            // 16 bit Data format
            pSPIx->DR = *((uint16_t *)pTxBuffer);
            pTxBuffer += 2;
            len -= 2;
        }
        else
        {
            // 8 bit data format
            pSPIx->DR = *pTxBuffer;
            pTxBuffer++;
            len--;
        }
    }
}

/**
 * @brief This function enable and disable SPI before data transfer.
 *
 * @param param1 Base address of SPI register
 * @param param2 Enable or Disable signal
 * @return None
 */

void SPI_EnableOrDisable(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{
    if (EnorDi)
        pSPIx->CR1 |= (1 << 6);
    else
    {
        pSPIx->CR1 &= ~(1 << 6);
    }
}

/**
 * @brief This function get status flags
 *
 * @param param1 SPI_SR registers status flags
 * @return either 1 or 0
 */

uint8_t SPI_StatusFlag(SPI_RegDef_t *pSPIx, uint16_t status_flag)
{
    return (pSPIx->SR & (1 << status_flag)) ? 1 : 0;
}

/**
 * @brief This function receives the data
 *
 * @param param1 Base address of SPI register
 * @param param2 Base address of Receiver buffer data
 * @param param3 length of Receive buffer data
 */

void SPI_Receive(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t len)
{

    while (len > 0)
    {
        while (getFlag(pSPIx->SR, SPI_Rx_FULL_FLAG) == RESET)
            ;

        if (getFlag(pSPIx->CR1, SPI_DFF_FLAG) == SET)
        {
            // 16 bit Data format
            *((uint16_t *)pRxBuffer) = pSPIx->DR;
            pRxBuffer += 2;
            len -= 2;
        }
        else
        {
            // 8 bit data format
            *pRxBuffer = pSPIx->DR;
            pRxBuffer++;
            len--;
        }
    }
}

void SPI_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnOrDi)
{
    if (EnOrDi)
    {
        NVIC_SET_PRIORITY(IRQNumber, IRQPriority);
        NVIC_ENABLE_IRQ(IRQNumber);
    }
    else
    {
        NVIC_DISABLE_IRQ(IRQNumber);
    }
}

uint8_t SPI_SendWithIT(SPI_Handle_t *pHandle, uint8_t *pTxBuffer, uint32_t len)
{
    // check if busy in communication
    if (pHandle->status == BUSY_STATUS)
    {
        return BUSY_STATUS;
    }
    // mark it as Busy
    pHandle->status = BUSY_STATUS;
    // if not then save pTxBuffer and len
    pHandle->pBuffer = pTxBuffer;
    pHandle->len = len;
    // also allow interrupt to occur by enabling TXEIE
    pHandle->pSPIx->CR2 |= (1 << SPI_TX_INT);
    // send ready status to indicate send is accepted
    return READY_STATUS;
}

uint8_t SPI_ReceiveWithIT(SPI_Handle_t *pHandle, uint8_t *pRxBuffer, uint32_t len)
{
    // check if busy in communication
    if (pHandle->status == BUSY_STATUS)
    {
        return BUSY_STATUS;
    }
    // mark it as Busy
    pHandle->status = BUSY_STATUS;
    // if not then save pTxBuffer and len
    pHandle->pBuffer = pRxBuffer;
    pHandle->len = len;
    // also allow interrupt to occur by enabling RXNEIE
    pHandle->pSPIx->CR2 |= (1 << SPI_RX_INT);
    // send ready status to indicate send is accepted
    return READY_STATUS;
}

void SPI_IRQHandling(SPI_Handle_t *pHandle)
{

    //  check if transmit buffer empty
    uint8_t tmp1 = (pHandle->pSPIx->SR & SPI_Tx_EMPTY_FLAG);
    uint8_t tmp2 = (pHandle->pSPIx->CR2 & (1 << SPI_TX_INT));
    if (tmp1 && tmp2)
    {
        SPI_send_data_handle(pHandle);
        return;
    }

    //  check if receive buffer not empty
    tmp1 = (pHandle->pSPIx->SR & SPI_Rx_FULL_FLAG);
    tmp2 = pHandle->pSPIx->CR2 & (1 << SPI_RX_INT);

    if (tmp1 && tmp2)
    {
        SPI_recv_data_handle(pHandle);
        return;
    }

    //  check if OVR Error
    tmp1 = pHandle->pSPIx->SR & (1 << SPI_OVR_ERR);
    tmp2 = pHandle->pSPIx->CR2 & (1 << SPI_ERR_INT);
    if (tmp1 && tmp2)
    {
        SPI_ovr_error_handle(pHandle);
        return;
    }

    //  check if Master mode fault
    tmp1 = pHandle->pSPIx->SR & (1 << SPI_MODF_ERR);
    tmp2 = pHandle->pSPIx->CR2 & (1 << SPI_ERR_INT);
    if (tmp1 && tmp2)
    {
        SPI_modf_error(pHandle);
        return;
    }
}

void SPI_ErrorInt(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{
    if (pSPIx == NULL)
        return;
    if (EnorDi)
    {
        pSPIx->CR2 |= (1 << SPI_ERR_INT);
    }
    else
    {
        pSPIx->CR2 &= ~(1 << SPI_ERR_INT);
    }
}

static void SPI_send_data_handle(SPI_Handle_t *pHandle)
{
    if (getFlag(pHandle->pSPIx->CR1, SPI_DFF_FLAG) == SET)
    {
        // 16 bit Data format
        pHandle->pSPIx->DR = *((uint16_t *)pHandle->pBuffer);
        pHandle->pBuffer += 2;
        pHandle->len -= 2;
    }
    else
    {
        // 8 bit data format
        pHandle->pSPIx->DR = *pHandle->pBuffer;
        pHandle->pBuffer++;
        pHandle->len--;
    }

    if (pHandle->len == 0)
    {
        pHandle->pBuffer = NULL;
        pHandle->len = 0;
        pHandle->status = READY_STATUS;
        pHandle->pSPIx->CR2 &= ~(1 << SPI_TX_INT);
        SPI_AppCallback(pHandle, CALLBACK_TX_COMPLETE);
    }
}

static void SPI_recv_data_handle(SPI_Handle_t *pHandle)
{
    if (getFlag(pHandle->pSPIx->CR1, SPI_DFF_FLAG) == SET)
    {
        // 16 bit Data format
        *((uint16_t *)pHandle->pBuffer) = pHandle->pSPIx->DR;
        pHandle->pBuffer += 2;
        pHandle->len -= 2;
    }
    else
    {
        // 8 bit data format
        *pHandle->pBuffer = pHandle->pSPIx->DR;
        pHandle->pBuffer++;
        pHandle->len--;
    }

    if (pHandle->len == 0)
    {
        pHandle->pBuffer = NULL;
        pHandle->len = 0;
        pHandle->status = READY_STATUS;
        pHandle->pSPIx->CR2 &= ~(1 << SPI_RX_INT);
        SPI_AppCallback(pHandle, CALLBACK_RX_COMPLETE);
    }
}

static void SPI_ovr_error_handle(SPI_Handle_t *pHandle)
{
    // implement ovr run error handling
    SPI_AppCallback(pHandle, CALLBACK_OVR_ERROR);
}

static void SPI_modf_error(SPI_Handle_t *pHandle)
{
    // implement modf error handling
    SPI_AppCallback(pHandle, CALLBACK_MODF_ERROR);
}

void SPI_Clear_OVR_ERROR(SPI_Handle_t *pHandle)
{
    if (pHandle != NULL && pHandle->pSPIx != NULL)
    {
        uint16_t tmp = pHandle->pSPIx->DR;
        tmp = pHandle->pSPIx->SR;
        (void)tmp;
    }
}

__attribute__((weak)) void SPI_AppCallback(SPI_Handle_t *pHandle, uint8_t status)
{
}
