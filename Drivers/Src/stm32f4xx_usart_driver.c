#include "stm32f4xx_usart_driver.h"

/*
 *   Helper function
 *   Baudrate setting
 */

static void USART_Set_Baudrate(USART_RegDef_t *pUSARTx, uint32_t baudrate)
{
    uint32_t pclk;
    // get peripheral clock
    if (pUSARTx == USART1 || pUSARTx == USART6)
    {
        pclk = Get_Peripheral_Clock(0); // apb2 clock
    }
    else
    {
        pclk = Get_Peripheral_Clock(1); // apb1 clock
    }

    // get sampling method
    uint8_t sampling_by8 = (pUSARTx->CR1 & (1 << 15)) > 0 ? 1 : 0;
    uint8_t sampling = (8 * (2 - sampling_by8));

    // calculate USARTDIV
    uint32_t USARTDIV = (pclk * 100) / (sampling * baudrate);

    // calculate mantissa
    uint32_t mantissa = USARTDIV / 100;

    // calculate fraction
    uint32_t fraction = (((USARTDIV % 100) * sampling) + 50) / 100;

    // set mantissa and fraction
    pUSARTx->BRR &= 0x0000;
    pUSARTx->BRR |= (fraction & 0x000F);
    pUSARTx->BRR |= (mantissa << 4);
}

/*
 * Peripheral Clock setup
 */
void USART_PeriClockControl(USART_RegDef_t *pUSARTx, uint8_t EnorDi)
{
    if (EnorDi)
    {
        if (pUSARTx == USART1)
            USART1_PCLK_EN();
        else if (pUSARTx == USART2)
            USART2_PCLK_EN();
        else if (pUSARTx == USART3)
            USART3_PCLK_EN();
        else if (pUSARTx == UART4)
            USART4_PCLK_EN();
        else if (pUSARTx == UART5)
            USART5_PCLK_EN();
        else if (pUSARTx == USART6)
            USART6_PCLK_EN();
    }
    else
    {
        if (pUSARTx == USART1)
            USART1_PCLK_DI();
        else if (pUSARTx == USART2)
            USART2_PCLK_DI();
        else if (pUSARTx == USART3)
            USART3_PCLK_DI();
        else if (pUSARTx == UART4)
            USART4_PCLK_DI();
        else if (pUSARTx == UART5)
            USART5_PCLK_DI();
        else if (pUSARTx == USART6)
            USART6_PCLK_DI();
    }
}

/*
 * Init and De-init
 */
void USART_Init(USART_Handle_t *pUSARTHandle)
{
    // setting up mode
    pUSARTHandle->pUSARTx->CR1 &= ~(1 << 3);
    pUSARTHandle->pUSARTx->CR1 &= ~(1 << 2);
    if (pUSARTHandle->usartConfig.Mode == USART_MODE_RX)
        pUSARTHandle->pUSARTx->CR1 |= (1 << 2);
    else if (pUSARTHandle->usartConfig.Mode == USART_MODE_TX)
        pUSARTHandle->pUSARTx->CR1 |= (1 << 3);
    else
    {
        pUSARTHandle->pUSARTx->CR1 |= (1 << 3);
        pUSARTHandle->pUSARTx->CR1 |= (1 << 2);
    }

    // setting up baud rate
    USART_Set_Baudrate(pUSARTHandle->pUSARTx, pUSARTHandle->usartConfig.Baud);

    // setting no of stop bits

    pUSARTHandle->pUSARTx->CR2 &= ~(0x3 << 12);
    pUSARTHandle->pUSARTx->CR2 |= (pUSARTHandle->usartConfig.NoOfStopBit << 12);

    // setting word length
    pUSARTHandle->pUSARTx->CR1 &= ~(1 << 12);
    pUSARTHandle->pUSARTx->CR1 |= (pUSARTHandle->usartConfig.WordLength << 12);

    // setting parity control

    if (pUSARTHandle->usartConfig.ParityControl == USART_PARITY_ENABLE_ODD)
    {
        pUSARTHandle->pUSARTx->CR1 |= (1 << 10);
        pUSARTHandle->pUSARTx->CR1 |= (1 << 9);
    }
    else if (pUSARTHandle->usartConfig.ParityControl == USART_PARITY_ENABLE_EVEN)
    {
        pUSARTHandle->pUSARTx->CR1 |= (1 << 10);
        pUSARTHandle->pUSARTx->CR1 &= ~(1 << 9);
    }
    else
    {
        pUSARTHandle->pUSARTx->CR1 &= ~(1 << 10);
    }

    // setting Hardware flow control
    if (pUSARTHandle->usartConfig.HardwareFlowControl == USART_HW_FLOW_CONTROL_CTS)
    {
        pUSARTHandle->pUSARTx->CR3 |= (1 << 9);
    }
    else if (pUSARTHandle->usartConfig.HardwareFlowControl == USART_HW_FLOW_CONTROL_RTS)
    {
        pUSARTHandle->pUSARTx->CR3 |= (1 << 8);
    }
    else if (pUSARTHandle->usartConfig.HardwareFlowControl == USART_HW_FLOW_CONTROL_CTS_RTS)
    {
        pUSARTHandle->pUSARTx->CR3 |= (1 << 9);
        pUSARTHandle->pUSARTx->CR3 |= (1 << 8);
    }
    else
    {
        pUSARTHandle->pUSARTx->CR3 &= ~(1 << 9);
        pUSARTHandle->pUSARTx->CR3 &= ~(1 << 8);
    }
}

void USART_DeInit(USART_RegDef_t *pUSARTx)
{
    if (pUSARTx == USART1)
        RCC->APB2RSTR |= (1 << 4);
    else if (pUSARTx == USART2)
        RCC->APB1RSTR |= (1 << 17);
    else if (pUSARTx == USART3)
        RCC->APB1RSTR |= (1 << 18);
    else if (pUSARTx == UART4)
        RCC->APB1RSTR |= (1 << 19);
    else if (pUSARTx == UART5)
        RCC->APB1RSTR |= (1 << 20);
    else if (pUSARTx == USART6)
        RCC->APB2RSTR |= (1 << 5);
}

/*
 * Data Send and Receive
 */
void USART_SendData(USART_RegDef_t *pUSARTx, uint8_t *pTxBuffer, uint32_t Len)
{

    while (Len > 0)
    {
        while (!USART_GetFlagStatus(pUSARTx, STATUS_FLAG_TX_EMPTY))
            ;
        if (IS_9_DATA_BITS_WORD(pUSARTx->CR1))
        {
            // 9 data bits

            if (IS_PARITY_ENABLE(pUSARTx->CR1))
            {
                // parity enable
                pUSARTx->DR = (*pTxBuffer & 0xFF);
                pTxBuffer++;
                Len--;
            }
            else
            {
                // parity disable
                uint16_t ptmp = ((*(uint16_t *)pTxBuffer) & 0x01FF);
                pUSARTx->DR = ptmp;
                pTxBuffer++;
                pTxBuffer++;
                Len -= 2;
            }
        }
        else
        {
            // 8 data bits
            if (IS_PARITY_ENABLE(pUSARTx->CR1))
            {
                // parity enable
                pUSARTx->DR = (*pTxBuffer & 0x7F);
                pTxBuffer++;
                Len--;
            }
            else
            {
                // parity disable
                pUSARTx->DR = (*pTxBuffer & 0xFF);
                pTxBuffer++;
                Len--;
            }
        }
    }
    while (!USART_GetFlagStatus(pUSARTx, STATUS_FLAG_TRANSMISSION_COMPLETE))
        ;
}

void USART_ReceiveData(USART_RegDef_t *pUSARTx, uint8_t *pRxBuffer, uint32_t Len)
{
    while (Len > 0)
    {
        while (!USART_GetFlagStatus(pUSARTx, STATUS_FLAG_RX_NOT_EMPTY))
            ;
        if (IS_9_DATA_BITS_WORD(pUSARTx->CR1))
        {
            // 9 data bits
            if (IS_PARITY_ENABLE(pUSARTx->CR1))
            {
                // parity enable
                *pRxBuffer = (pUSARTx->DR & 0xFF);
                pRxBuffer++;
                Len--;
            }
            else
            {
                // parity disable
                *(uint16_t *)pRxBuffer = (pUSARTx->DR & 0x01FF);
                pRxBuffer++;
                pRxBuffer++;
                Len--;
                Len--;
            }
        }
        else
        {
            // 8 data bits
            if (IS_PARITY_ENABLE(pUSARTx->CR1))
            {
                // parity enable
                *pRxBuffer = (pUSARTx->DR & 0x7F);
                pRxBuffer++;
                Len--;
            }
            else
            {
                // parity disable
                *pRxBuffer = (pUSARTx->DR & 0xFF);
                pRxBuffer++;
                Len--;
            }
        }
    }
}

uint8_t USART_SendDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len)
{
    // checking if USART is busy or not
    if (pUSARTHandle->statusTx == USART_STATUS_BUSY)
        return USART_STATUS_BUSY;

    // make status as busy

    pUSARTHandle->statusTx = USART_STATUS_BUSY;

    // copy buffer address and length

    pUSARTHandle->pTxBuffer = pTxBuffer;
    pUSARTHandle->lenTx = Len;

    // enable TXEIE interrupt

    pUSARTHandle->pUSARTx->CR1 |= (1 << 7);

    // enable TCIE interrupt

    pUSARTHandle->pUSARTx->CR1 |= (1 << 6);

    // send ready status
    return USART_STATUS_READY;
}

uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len)
{
    // checking if USART is busy or not
    if (pUSARTHandle->statusRx == USART_STATUS_BUSY)
        return USART_STATUS_BUSY;

    // make status as busy

    pUSARTHandle->statusRx = USART_STATUS_BUSY;

    // copy buffer address and length

    pUSARTHandle->pRxBuffer = pRxBuffer;
    pUSARTHandle->lenRx = Len;

    // enable RXNEIE interrupt

    pUSARTHandle->pUSARTx->CR1 |= (1 << 5);

    // send ready status
    return USART_STATUS_READY;
}

/*
 * IRQ Configuration and ISR handling
 */
void USART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnOrDi)
{
    if (EnOrDi)
    {

        NVIC_ENABLE_IRQ(IRQNumber);
    }
    else
    {
        NVIC_DISABLE_IRQ(IRQNumber);
    }
}

void USART_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
{
    NVIC_SET_PRIORITY(IRQNumber, IRQPriority);
}

void send_data(USART_Handle_t *pHandle)
{
    if (pHandle->lenTx == 0 || pHandle->pTxBuffer == NULL)
    {
        USART_ApplicationEventCallback(pHandle, USART_CALLBACK_DATA_SEND_ERROR);
        return;
    }

    if (IS_9_DATA_BITS_WORD(pHandle->pUSARTx->CR1))
    {
        // 9 data bits

        if (IS_PARITY_ENABLE(pHandle->pUSARTx->CR1))
        {
            // parity enable
            pHandle->pUSARTx->DR = (*pHandle->pTxBuffer & 0xFF);
            pHandle->pTxBuffer++;
            pHandle->lenTx--;
        }
        else
        {
            // parity disable
            uint16_t ptmp = ((*(uint16_t *)pHandle->pTxBuffer) & 0x01FF);
            pHandle->pUSARTx->DR = ptmp;
            pHandle->pTxBuffer++;
            pHandle->pTxBuffer++;
            pHandle->lenTx -= 2;
        }
    }
    else
    {
        // 8 data bits
        if (IS_PARITY_ENABLE(pHandle->pUSARTx->CR1))
        {
            // parity enable
            pHandle->pUSARTx->DR = (*pHandle->pTxBuffer & 0x7F);
            pHandle->pTxBuffer++;
            pHandle->lenTx--;
        }
        else
        {
            // parity disable
            pHandle->pUSARTx->DR = (*pHandle->pTxBuffer & 0xFF);
            pHandle->pTxBuffer++;
            pHandle->lenTx--;
        }
    }

    if (pHandle->lenTx == 0)
    {
        // data send completed so disable TXEIE interrupt and send callback
        pHandle->pUSARTx->CR1 &= ~(1 << 7);
        USART_ApplicationEventCallback(pHandle, USART_CALLBACK_DATA_SEND_COMPLETED);
    }
}

void transmission_completed(USART_Handle_t *pHandle)
{
    if (pHandle->statusTx == USART_STATUS_BUSY && pHandle->lenTx == 0)
    {
        pHandle->pUSARTx->SR &= ~(1 << 6);  // clearing TC bit
        pHandle->pUSARTx->CR1 &= ~(1 << 6); // clearing TCIE bit
        pHandle->pTxBuffer = NULL;
        pHandle->lenTx = 0;
        pHandle->statusTx = USART_STATUS_READY;
        USART_ApplicationEventCallback(pHandle, USART_CALLBACK_TRANMISSION_COMPLETED);
    }
}

void recv_data(USART_Handle_t *pHandle)
{
    if (pHandle->lenRx == 0 || pHandle->pRxBuffer == NULL)
    {
        USART_ApplicationEventCallback(pHandle, USART_CALLBACK_DATA_RECV_ERROR);
        return;
    }

    if (IS_9_DATA_BITS_WORD(pHandle->pUSARTx->CR1))
    {
        // 9 data bits
        if (IS_PARITY_ENABLE(pHandle->pUSARTx->CR1))
        {
            // parity enable
            *pHandle->pRxBuffer = (pHandle->pUSARTx->DR & 0xFF);
            pHandle->pRxBuffer++;
            pHandle->lenRx--;
        }
        else
        {
            // parity disable
            *(uint16_t *)pHandle->pRxBuffer = (pHandle->pUSARTx->DR & 0x01FF);
            pHandle->pRxBuffer++;
            pHandle->pRxBuffer++;
            pHandle->lenRx--;
            pHandle->lenRx--;
        }
    }
    else
    {
        // 8 data bits
        if (IS_PARITY_ENABLE(pHandle->pUSARTx->CR1))
        {
            // parity enable
            *pHandle->pRxBuffer = (pHandle->pUSARTx->DR & 0x7F);
            pHandle->pRxBuffer++;
            pHandle->lenRx--;
        }
        else
        {
            // parity disable
            *pHandle->pRxBuffer = (pHandle->pUSARTx->DR & 0xFF);
            pHandle->pRxBuffer++;
            pHandle->lenRx--;
        }
    }
    if (pHandle->lenRx == 0)
    {
        // clear the RXNEIE bit and send callback and clear the buffer,len and status
        pHandle->pUSARTx->CR1 &= ~(1 << 5);
        pHandle->lenRx = 0;
        pHandle->pRxBuffer = NULL;
        pHandle->statusRx = USART_STATUS_READY;
        USART_ApplicationEventCallback(pHandle, USART_CALLBACK_DATA_RECV_COMPLETED);
    }
}

void ore_error(USART_Handle_t *pHandle)
{
    USART_ApplicationEventCallback(pHandle, USART_CALLBACK_ORE);
}

void idle_report(USART_Handle_t *pHandle)
{
    USART_ApplicationEventCallback(pHandle, USART_CALLBACK_IDLE);
}

void parity_error(USART_Handle_t *pHandle)
{
    USART_ApplicationEventCallback(pHandle, USART_CALLBACK_PARITY_ERROR);
}

void USART_IRQHandling(USART_Handle_t *pHandle)
{
    // check if Transmit data register is empty

    uint16_t temp1 = (pHandle->pUSARTx->SR & (1 << 7));
    uint16_t temp2 = (pHandle->pUSARTx->CR1 & (1 << 7));
    if (temp1 && temp2)
    {
        send_data(pHandle);
        return;
    }

    // check if Tranmission completed
    temp1 = (pHandle->pUSARTx->SR & (1 << 6));
    temp2 = (pHandle->pUSARTx->CR1 & (1 << 6));
    if (temp1 && temp2)
    {
        transmission_completed(pHandle);
        return;
    }

    // check if Received data ready to read
    temp1 = (pHandle->pUSARTx->SR & (1 << 5));
    temp2 = (pHandle->pUSARTx->CR1 & (1 << 5));
    if (temp1 && temp2)
    {
        recv_data(pHandle);
        return;
    }

    // check if overrun error detected
    temp1 = (pHandle->pUSARTx->SR & (1 << 3));
    temp2 = (pHandle->pUSARTx->CR1 & (1 << 5));
    if (temp1 && temp2)
    {
        ore_error(pHandle);
        return;
    }

    // check if IDLE line detected
    temp1 = (pHandle->pUSARTx->SR & (1 << 4));
    temp2 = (pHandle->pUSARTx->CR1 & (1 << 4));
    if (temp1 && temp2)
    {
        idle_report(pHandle);
        return;
    }
    // check if parity Error
    temp1 = (pHandle->pUSARTx->SR & (1 << 0));
    temp2 = (pHandle->pUSARTx->CR1 & (1 << 8));
    if (temp1 && temp2)
    {
        parity_error(pHandle);
        return;
    }
    // Todo: check if CTS flag is set

    // Todo: check if Break flag is set

    // Todo: check if Noise,overrun and framming error in multibuffer communication
}

/*
 * Other Peripheral Control APIs
 */
void USART_PeripheralControl(USART_RegDef_t *pUSARTx, uint8_t EnOrDi)
{
    if (EnOrDi)
    { // USART Enable
        pUSARTx->CR1 |= (1 << 13);
    }
    else
    { // USART Disable
        while (!USART_GetFlagStatus(pUSARTx, STATUS_FLAG_TRANSMISSION_COMPLETE))
            ;
        pUSARTx->CR1 &= ~(1 << 13);
    }
}

uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx, uint32_t FlagName)
{
    return (pUSARTx->SR & (1 << FlagName)) > 0 ? 1 : 0;
}

void USART_ClearFlag(USART_RegDef_t *pUSARTx, uint16_t StatusFlagName)
{
}

/*
 * Application callback
 */
__attribute__((weak)) void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle, uint8_t AppEv)
{
}