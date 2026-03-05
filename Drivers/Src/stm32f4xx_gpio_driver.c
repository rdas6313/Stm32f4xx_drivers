#include "stm32f4xx_gpio_driver.h"

// GPIO API

// GPIO Clock enable and disable

/**
 * @brief This function enable and disable peripheral clock
 *
 * @param param1 Base address of GPIO port
 * @param param2 Enable or Disable value
 * @return None
 */

void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnOrDi)
{
    if (EnOrDi == ENABLE)
    {
        if (pGPIOx == GPIOA)
            GPIOA_PCLK_EN();
        else if (pGPIOx == GPIOB)
            GPIOB_PCLK_EN();
        else if (pGPIOx == GPIOC)
            GPIOC_PCLK_EN();
        else if (pGPIOx == GPIOD)
            GPIOD_PCLK_EN();
        else if (pGPIOx == GPIOE)
            GPIOE_PCLK_EN();
        else if (pGPIOx == GPIOF)
            GPIOF_PCLK_EN();
        else if (pGPIOx == GPIOG)
            GPIOG_PCLK_EN();
        else if (pGPIOx == GPIOH)
            GPIOH_PCLK_EN();
        else if (pGPIOx == GPIOI)
            GPIOI_PCLK_EN();
    }
    else
    {
        if (pGPIOx == GPIOA)
            GPIOA_PCLK_DI();
        else if (pGPIOx == GPIOB)
            GPIOB_PCLK_DI();
        else if (pGPIOx == GPIOC)
            GPIOC_PCLK_DI();
        else if (pGPIOx == GPIOD)
            GPIOD_PCLK_DI();
        else if (pGPIOx == GPIOE)
            GPIOE_PCLK_DI();
        else if (pGPIOx == GPIOF)
            GPIOF_PCLK_DI();
        else if (pGPIOx == GPIOG)
            GPIOG_PCLK_DI();
        else if (pGPIOx == GPIOH)
            GPIOH_PCLK_DI();
        else if (pGPIOx == GPIOI)
            GPIOI_PCLK_DI();
    }
}

// GPIO Init and De-init

/**
 * @brief This function helps to configure GPIO pin
 *
 * @param param1 pointer of GPIO_Handle_t struct
 * @return None
 */

void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{

    if (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber >= MAX_GPIO_PIN)
        return;

    uint8_t temp = 0;
    if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
    {
        // GPIO Pin mode setting
        temp = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber * 2;
        pGPIOHandle->pGPIOx->MODER &= ~(0x3 << temp);                                     // reset the mode of correspoinding pin
        pGPIOHandle->pGPIOx->MODER |= (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << temp); // set the mode
    }
    else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode < GPIO_MAX_MODE)
    {
        if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_INT_RISE)
        {
            EXTI->RTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
            EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
        }
        else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_INT_FALL)
        {
            EXTI->FTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
            EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
        }
        else
        {
            EXTI->RTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
            EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
            EXTI->FTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
            EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
        }
        EXTI->IMR &= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
        EXTI->IMR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

        // Enable SYSCFG clock
        SYSCFG_PCLK_EN();
        // configuring SYSTEM Config controller
        uint8_t index = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 4;
        uint8_t shift = (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 4) * 4;
        uint8_t port = EXTI_GPIO_PORT_MAP(pGPIOHandle->pGPIOx);
        SYSCFG->EXTICR[index] |= (port << shift);
    }

    if (pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType < GPIO_MAX_OP_TYPE)
    {
        // GPIO OUTput type setting
        pGPIOHandle->pGPIOx->OTYPER &= ~(1 << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));                                         // reset the Output type
        pGPIOHandle->pGPIOx->OTYPER |= (pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)); // set the Output type
    }

    if (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed < GPIO_MAX_OP_SPEED)
    {
        // GPIO speed setting
        temp = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber * 2;
        pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x3 << temp);                                      // reset the speed of correspoinding pin
        pGPIOHandle->pGPIOx->OSPEEDR |= (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << temp); // set the speed
    }

    if (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl < GPIO_MAX_PUPDR)
    {
        // GPIO pull up/pull down setting
        temp = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber * 2;
        pGPIOHandle->pGPIOx->PUPDR &= ~(0x3 << temp);                                            // reset the PULL-UP/PULL-DOWN of correspoinding pin
        pGPIOHandle->pGPIOx->PUPDR |= (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl << temp); // set the PULL-UP/PULL-DOWN
    }

    if (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode < GPIO_MAX_ALTF && pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN)
    {
        // GPIO Alternate function setting
        temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 8) * 4;
        uint8_t index = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 8;
        pGPIOHandle->pGPIOx->AFR[index] &= ~(0xF << temp);                                           // reset the alternate function
        pGPIOHandle->pGPIOx->AFR[index] |= (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << temp); // set the alternate function
    }
}

/**
 * @brief This function resets the GPIO Port x registers.(After You make Reset register 1, change it to 0, otherwise it will stay in reset state )
 *
 * @param param1 Base address of GPIOx
 * @return None
 */

void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)
{
    if (pGPIOx == GPIOA)
        GPIO_PORT_A_RST();
    else if (pGPIOx == GPIOB)
        GPIO_PORT_B_RST();
    else if (pGPIOx == GPIOC)
        GPIO_PORT_C_RST();
    else if (pGPIOx == GPIOD)
        GPIO_PORT_D_RST();
    else if (pGPIOx == GPIOE)
        GPIO_PORT_E_RST();
    else if (pGPIOx == GPIOF)
        GPIO_PORT_F_RST();
    else if (pGPIOx == GPIOG)
        GPIO_PORT_G_RST();
    else if (pGPIOx == GPIOH)
        GPIO_PORT_H_RST();
    else if (pGPIOx == GPIOI)
        GPIO_PORT_I_RST();
}

// Data read and write

/**
 * @brief It reads from certain GPIO input pin of a GPIO port
 *
 * @param param1 Base address of GPIO port
 * @param param2 From which pin you want to read from
 * @return 8 bit unsigned int, either 0 or 1
 */

uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber)
{
    return (uint8_t)((pGPIOx->IDR >> pinNumber) & 0x00000001);
}

/**
 * @brief It reads from certain GPIO port
 *
 * @param param1 Base address of GPIO port
 * @return 16 bit unsigned int, refers first 16 bit of input data register
 */

uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{
    return (uint16_t)pGPIOx->IDR;
}

/**
 * @brief It writes a value (0 or 1) to certain pin of certain GPIO port output register
 *
 * @param param1 Base address of GPIO port
 * @param param2 To which pin you want to write
 * @param param2 The value you want to write
 * @return None
 */

void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber, uint8_t value)
{
    pGPIOx->ODR &= (~(1 << pinNumber));
    pGPIOx->ODR |= (value << pinNumber);
}

/**
 * @brief It writes value to certain GPIO port output register
 *
 * @param param1 Base address of GPIO port
 * @param param2 The value you want to write
 * @return None
 */

void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t value)
{
    pGPIOx->ODR = value;
}

/**
 * @brief Function description
 *
 * @param param1 Description
 * @param param2 Description
 * @return Return type and meaning
 */

void GPIO_ToggleOuputPin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber)
{
    pGPIOx->ODR ^= (1 << pinNumber);
}

// IRQ Configuration and ISR handling

/**
 * @brief This function configure IRQ of GPIO
 *
 * @param param1 IRQ number of IRQ
 * @param param2 IRQ priority of IRQ
 * @param param2 Enable or disable IRQ
 * @return None
 */

void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnOrDi)
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

/**
 * @brief This function clears Pending IRQs
 *
 * @param param1 Pin number of GPIO
 * @return None
 */

void GPIO_IRQHandling(uint8_t pinNumber)
{
    if (EXTI->PR & (1 << pinNumber))
        EXTI->PR |= (1 << pinNumber);
}