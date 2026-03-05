
#ifndef _STM32_HEAD
#define _STM32_HEAD
#include <stdint.h>
#include <stddef.h>

/*NVIC BASE ADDRESSES*/

#define NVIC_ISER_BASEADDR 0xE000E100U
#define NVIC_ICER_BASEADDR 0XE000E180U
#define NVIC_ISPR_BASEADDR 0XE000E200U
#define NVIC_ICPR_BASEADDR 0XE000E280U
#define NVIC_IABR_BASEADDR 0xE000E300U
#define NVIC_IPR_BASEADDR 0xE000E400U

#define NVIC_ISER(reg) (NVIC_ISER_BASEADDR + ((reg) * 4)) // NVIC Interrupt set enable register
#define NVIC_ICER(reg) (NVIC_ICER_BASEADDR + ((reg) * 4)) // NVIC Interrupt clear enable register
#define NVIC_IPR(reg) (NVIC_IPR_BASEADDR + ((reg) * 4))   // NVIC Priority register

// NVIC INTERUPT ENABLE
#define NVIC_ENABLE_IRQ(IRQNumber) (*((uint32_t *)NVIC_ISER(IRQNumber / 32)) |= (1 << (IRQNumber % 32)))

// NVIC INTERUPT DISABLE
#define NVIC_DISABLE_IRQ(IRQNumber) (*((uint32_t *)NVIC_IPR(IRQNumber / 32)) |= (1 << (IRQNumber % 32)))

// Nvic Unused bit for this MCU
#define NVIC_PRIORITY_UNUSED_BITS 4

// NVIC INTERRUPT SET PRIORITY
#define NVIC_SET_PRIORITY(irq, prio) (*((uint32_t *)NVIC_IPR(irq / 4)) |= (prio << (((irq % 4) * 8) + (8 - NVIC_PRIORITY_UNUSED_BITS))))

// Base addresses of SRAM and FLASH memory

#define FLASH_BASEADDR 0x80000000U
#define SRAM1_BASEADDR 0x20000000U
#define SRAM2_BASEADDR 0x2001C000U
#define ROM_BASEADDR 0x1FFF0000U
#define SRAM_BASEADDR SRAM1_BASEADDR

// AHBx and APBx Bus peripheral base addresses

#define PERIPH_BASEADDR 0x40000000U
#define APB1PERIPH_BASEADDR PERIPH_BASEADDR
#define APB2PERIPH_BASEADDR 0x40010000U
#define AHB1PERIPH_BASEADDR 0x40020000U
#define AHB2PERIPH_BASEADDR 0x50000000U
#define AHB3PERIPH_BASEADDR 0xA0000000U

// AHB1 peripherals Base address

#define GPIOA_BASEADDR (AHB1PERIPH_BASEADDR + 0x0000)
#define GPIOB_BASEADDR (AHB1PERIPH_BASEADDR + 0x0400)
#define GPIOC_BASEADDR (AHB1PERIPH_BASEADDR + 0x0800)
#define GPIOD_BASEADDR (AHB1PERIPH_BASEADDR + 0x0C00)
#define GPIOE_BASEADDR (AHB1PERIPH_BASEADDR + 0x1000)
#define GPIOF_BASEADDR (AHB1PERIPH_BASEADDR + 0x1400)
#define GPIOG_BASEADDR (AHB1PERIPH_BASEADDR + 0x1800)
#define GPIOH_BASEADDR (AHB1PERIPH_BASEADDR + 0x1C00)
#define GPIOI_BASEADDR (AHB1PERIPH_BASEADDR + 0x2000)
#define GPIOI_BASEADDR (AHB1PERIPH_BASEADDR + 0x2000)
#define GPIOI_BASEADDR (AHB1PERIPH_BASEADDR + 0x2000)
#define GPIOJ_BASEADDR (AHB1PERIPH_BASEADDR + 0x2400)
#define GPIOK_BASEADDR (AHB1PERIPH_BASEADDR + 0x2800)
#define RCC_BASEADDR (AHB1PERIPH_BASEADDR + 0x3800)

// APB1 peripherals Base address

#define I2C1_BASEADDR (APB1PERIPH_BASEADDR + 0x5400)
#define I2C2_BASEADDR (APB1PERIPH_BASEADDR + 0x5800)
#define I2C3_BASEADDR (APB1PERIPH_BASEADDR + 0x5C00)
#define TIM6_BASEADDR (APB1PERIPH_BASEADDR + 0x1000)
#define TIM7_BASEADDR (APB1PERIPH_BASEADDR + 0x1400)

// SPI peripherals Base address

#define SPI2_BASEADDR (APB1PERIPH_BASEADDR + 0x3800)
#define SPI3_BASEADDR (APB1PERIPH_BASEADDR + 0x3C00)
#define SPI1_BASEADDR (APB2PERIPH_BASEADDR + 0x3000)
#define SPI4_BASEADDR (APB2PERIPH_BASEADDR + 0x3400)
#define SPI5_BASEADDR (APB2PERIPH_BASEADDR + 0x5000)
#define SPI6_BASEADDR (APB2PERIPH_BASEADDR + 0x5400)

// USART BASE ADDRESS

#define USART2_BASEADDR (APB1PERIPH_BASEADDR + 0x4400)
#define USART3_BASEADDR (APB1PERIPH_BASEADDR + 0x4800)
#define UART4_BASEADDR (APB1PERIPH_BASEADDR + 0x4C00)
#define UART5_BASEADDR (APB1PERIPH_BASEADDR + 0x5000)
#define UART7_BASEADDR (APB1PERIPH_BASEADDR + 0x7800)
#define UART8_BASEADDR (APB1PERIPH_BASEADDR + 0x7C00)

// APB2 peripherals Base address
#define USART1_BASEADDR (APB2PERIPH_BASEADDR + 0x1000)
#define USART6_BASEADDR (APB2PERIPH_BASEADDR + 0x1400)

#define EXTI_BASEADDR (APB2PERIPH_BASEADDR + 0x3C00)
#define SYSCFG_BASEADDR (APB2PERIPH_BASEADDR + 0x3800)

/* GPIO Register definations */

typedef struct
{
    volatile uint32_t MODER;   // GPIO port mode register
    volatile uint32_t OTYPER;  // GPIO port output type register
    volatile uint32_t OSPEEDR; // GPIO port output speed register
    volatile uint32_t PUPDR;   // GPIO port pull-up/pull-down register
    volatile uint32_t IDR;     // GPIO port input data register
    volatile uint32_t ODR;     // GPIO port output data register
    volatile uint32_t BSRR;    // GPIO port bit set/reset register
    volatile uint32_t LCKR;    // GPIO port configuration lock register
    volatile uint32_t AFR[2];  // AFR[0]-> GPIO alternate function low register AFR[1]-> GPIO alternate function high register
} GPIO_RegDef_t;

/* Peripheral definations (peripherals base address type casted)*/
#define GPIOA ((GPIO_RegDef_t *)GPIOA_BASEADDR)
#define GPIOB ((GPIO_RegDef_t *)GPIOB_BASEADDR)
#define GPIOC ((GPIO_RegDef_t *)GPIOC_BASEADDR)
#define GPIOD ((GPIO_RegDef_t *)GPIOD_BASEADDR)
#define GPIOE ((GPIO_RegDef_t *)GPIOE_BASEADDR)
#define GPIOF ((GPIO_RegDef_t *)GPIOF_BASEADDR)
#define GPIOG ((GPIO_RegDef_t *)GPIOG_BASEADDR)
#define GPIOH ((GPIO_RegDef_t *)GPIOH_BASEADDR)
#define GPIOI ((GPIO_RegDef_t *)GPIOI_BASEADDR)
#define GPIOJ ((GPIO_RegDef_t *)GPIOJ_BASEADDR)
#define GPIOK ((GPIO_RegDef_t *)GPIOK_BASEADDR)

typedef struct
{
    volatile uint32_t CR;           // RCC clock control register
    volatile uint32_t PLLCFGR;      // RCC PLL configuration register
    volatile uint32_t CFGR;         // RCC clock configuration register
    volatile uint32_t CIR;          // RCC clock interrupt register
    volatile uint32_t AHB1RSTR;     // RCC AHB1 peripheral reset register
    volatile uint32_t AHB2RSTR;     // RCC AHB2 peripheral reset register
    volatile uint32_t AHB3RSTR;     // RCC AHB3 peripheral reset register
    volatile uint32_t RESERVED1;    // RESERVED
    volatile uint32_t APB1RSTR;     // RCC APB1 peripheral reset register
    volatile uint32_t APB2RSTR;     // RCC APB2 peripheral reset register
    volatile uint32_t RESERVED2[2]; // RESERVED
    volatile uint32_t AHB1ENR;      // RCC AHB1 peripheral clock enable register
    volatile uint32_t AHB2ENR;      // RCC AHB2 peripheral clock enable register
    volatile uint32_t AHB3ENR;      // RCC AHB3 peripheral clock enable register
    volatile uint32_t RESERVED3;    // RESERVED
    volatile uint32_t APB1ENR;      // RCC APB1 peripheral clock enable register
    volatile uint32_t APB2ENR;      // RCC APB2 peripheral clock enable register
    volatile uint32_t RESERVED4[2]; // RESERVED
    volatile uint32_t AHB1LPENR;    // RCC AHB1 peripheral clock enable in low power mode register
    volatile uint32_t AHB2LPENR;    // RCC AHB2 peripheral clock enable in low power mode register
    volatile uint32_t AHB3LPENR;    // RCC AHB3 peripheral clock enable in low power mode register
    volatile uint32_t RESERVED5;    // RESERVED
    volatile uint32_t APB1LPENR;    // RCC APB1 peripheral clock enable in low power mode register
    volatile uint32_t APB2LPENR;    // RCC APB2 peripheral clock enabled in low power mode
    volatile uint32_t RESERVED6[2]; // RESERVED
    volatile uint32_t BDCR;         // RCC Backup domain control register
    volatile uint32_t CSR;          // RCC clock control & status register
    volatile uint32_t RESERVED7[2]; // RESERVED
    volatile uint32_t SSCGR;        // RCC spread spectrum clock generation register
    volatile uint32_t PLLI2SCFGR;   // RCC PLLI2S configuration register

} RCC_RegDef_t;

#define RCC ((RCC_RegDef_t *)RCC_BASEADDR)

/*GPIO Peripheral enable*/
#define GPIOA_PCLK_EN() (RCC->AHB1ENR |= (1 << 0))
#define GPIOB_PCLK_EN() (RCC->AHB1ENR |= (1 << 1))
#define GPIOC_PCLK_EN() (RCC->AHB1ENR |= (1 << 2))
#define GPIOD_PCLK_EN() (RCC->AHB1ENR |= (1 << 3))
#define GPIOE_PCLK_EN() (RCC->AHB1ENR |= (1 << 4))
#define GPIOF_PCLK_EN() (RCC->AHB1ENR |= (1 << 5))
#define GPIOG_PCLK_EN() (RCC->AHB1ENR |= (1 << 6))
#define GPIOH_PCLK_EN() (RCC->AHB1ENR |= (1 << 7))
#define GPIOI_PCLK_EN() (RCC->AHB1ENR |= (1 << 8))

/*I2C Peripheral enable*/

#define I2C1_PCLK_EN() (RCC->APB1ENR |= (1 << 21))
#define I2C2_PCLK_EN() (RCC->APB1ENR |= (1 << 22))
#define I2C3_PCLK_EN() (RCC->APB1ENR |= (1 << 23))

/*SPI Peripheral enable*/

#define SPI1_PCLK_EN() (RCC->APB2ENR |= (1 << 12))
#define SPI2_PCLK_EN() (RCC->APB1ENR |= (1 << 14))
#define SPI3_PCLK_EN() (RCC->APB1ENR |= (1 << 15))

/*UART Peripheral enable */
#define USART1_PCLK_EN() (RCC->APB2ENR |= (1 << 4))
#define USART2_PCLK_EN() (RCC->APB1ENR |= (1 << 17))
#define USART3_PCLK_EN() (RCC->APB1ENR |= (1 << 18))
#define USART4_PCLK_EN() (RCC->APB1ENR |= (1 << 19))
#define USART5_PCLK_EN() (RCC->APB1ENR |= (1 << 20))
#define USART6_PCLK_EN() (RCC->APB2ENR |= (1 << 5))

/*SYSCFG Peripheral enable*/
#define SYSCFG_PCLK_EN() (RCC->APB2ENR |= (1 << 14))

/* Timers enable */
#define TIM6_PCLK_EN() (RCC->APB1ENR |= (1 << 4))
#define TIM7_PCLK_EN() (RCC->APB1ENR |= (1 << 5))

/*GPIO Peripheral disable*/

#define GPIOA_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 0))
#define GPIOB_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 1))
#define GPIOC_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 2))
#define GPIOD_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 3))
#define GPIOE_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 4))
#define GPIOF_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 5))
#define GPIOG_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 6))
#define GPIOH_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 7))
#define GPIOI_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 8))

/*I2C Peripheral disable*/

#define I2C1_PCLK_DI() (RCC->APB1ENR &= ~(1 << 21))
#define I2C2_PCLK_DI() (RCC->APB1ENR &= ~(1 << 22))
#define I2C3_PCLK_DI() (RCC->APB1ENR &= ~(1 << 23))

/*SPI Peripheral disable*/

#define SPI1_PCLK_DI() (RCC->APB2ENR &= ~(1 << 12))
#define SPI2_PCLK_DI() (RCC->APB1ENR &= ~(1 << 14))
#define SPI3_PCLK_DI() (RCC->APB1ENR &= ~(1 << 15))

/*UART Peripheral disable */
#define USART1_PCLK_DI() (RCC->APB2ENR &= ~(1 << 4))
#define USART2_PCLK_DI() (RCC->APB1ENR &= ~(1 << 17))
#define USART3_PCLK_DI() (RCC->APB1ENR &= ~(1 << 18))
#define USART4_PCLK_DI() (RCC->APB1ENR &= ~(1 << 19))
#define USART5_PCLK_DI() (RCC->APB1ENR &= ~(1 << 20))
#define USART6_PCLK_DI() (RCC->APB2ENR &= ~(1 << 5))

/*SYSCFG Peripheral disable*/
#define SYSCFG_PCLK_DI() (RCC->APB2ENR &= ~(1 << 14))

/* Timers disable */
#define TIM6_PCLK_DI() (RCC->APB1ENR &= ~(1 << 4))
#define TIM7_PCLK_DI() (RCC->APB1ENR &= ~(1 << 5))

// generic macros
#define ENABLE 1
#define DISABLE 0
#define SET ENABLE
#define RESET DISABLE

#define MAX_GPIO_PIN 16

// GPIO peripheral register reset
#define GPIO_PORT_A_RST()             \
    do                                \
    {                                 \
        RCC->AHB1RSTR |= (1 << 0);    \
        RCC->AHB1RSTR &= (~(1 << 0)); \
    } while (0)
#define GPIO_PORT_B_RST()             \
    do                                \
    {                                 \
        RCC->AHB1RSTR |= (1 << 1);    \
        RCC->AHB1RSTR &= (~(1 << 1)); \
    } while (0)
#define GPIO_PORT_C_RST()             \
    do                                \
    {                                 \
        RCC->AHB1RSTR |= (1 << 2);    \
        RCC->AHB1RSTR &= (~(1 << 2)); \
    } while (0)
#define GPIO_PORT_D_RST()             \
    do                                \
    {                                 \
        RCC->AHB1RSTR |= (1 << 3);    \
        RCC->AHB1RSTR &= (~(1 << 3)); \
    } while (0)
#define GPIO_PORT_E_RST()             \
    do                                \
    {                                 \
        RCC->AHB1RSTR |= (1 << 4);    \
        RCC->AHB1RSTR &= (~(1 << 4)); \
    } while (0)
#define GPIO_PORT_F_RST()             \
    do                                \
    {                                 \
        RCC->AHB1RSTR |= (1 << 5);    \
        RCC->AHB1RSTR &= (~(1 << 5)); \
    } while (0)
#define GPIO_PORT_G_RST()             \
    do                                \
    {                                 \
        RCC->AHB1RSTR |= (1 << 6);    \
        RCC->AHB1RSTR &= (~(1 << 6)); \
    } while (0)
#define GPIO_PORT_H_RST()             \
    do                                \
    {                                 \
        RCC->AHB1RSTR |= (1 << 7);    \
        RCC->AHB1RSTR &= (~(1 << 7)); \
    } while (0)
#define GPIO_PORT_I_RST()             \
    do                                \
    {                                 \
        RCC->AHB1RSTR |= (1 << 8);    \
        RCC->AHB1RSTR &= (~(1 << 8)); \
    } while (0)


// TIMERS Peripheral reset
#define TIM6_RST() do{ RCC->APB1RSTR |= (1<<4); RCC->APB1RSTR &= (~(1<<4)); }while(0)
#define TIM7_RST() do{ RCC->APB1RSTR |= (1<<5); RCC->APB1RSTR &= (~(1<<5)); }while(0)

// EXTI registers definations
typedef struct
{
    volatile uint32_t IMR;   // Interrupt mask register
    volatile uint32_t EMR;   // Event mask register
    volatile uint32_t RTSR;  // Rising trigger selection register
    volatile uint32_t FTSR;  // Falling trigger selection register
    volatile uint32_t SWIER; // Software interrupt event register
    volatile uint32_t PR;    // Pending register
} EXTI_RegDef_t;

// EXTI Typecasted
#define EXTI ((EXTI_RegDef_t *)EXTI_BASEADDR)

// EXTI Interrupt Clear
#define EXTI_Clear_INT(pin) (EXTI->PR |= (1 << pin))

// System configuration controller
typedef struct
{
    volatile uint32_t MEMRMP;    // memory remap register
    volatile uint32_t PMC;       // peripheral mode configuration register
    volatile uint32_t EXTICR[4]; // external interrupt configuration registers
    volatile uint32_t CMPCR;     // Compensation cell control register
} SYSCFG_RegDef_t;

#define SYSCFG ((SYSCFG_RegDef_t *)SYSCFG_BASEADDR)

// SYSCFG port to exti value mapping
#define EXTI_GPIO_PORT_MAP(port) ((port == GPIOA) ? 0 : (port == GPIOB) ? 1 \
                                                    : (port == GPIOC)   ? 2 \
                                                    : (port == GPIOD)   ? 3 \
                                                    : (port == GPIOE)   ? 4 \
                                                    : (port == GPIOF)   ? 5 \
                                                    : (port == GPIOG)   ? 6 \
                                                    : (port == GPIOH)   ? 7 \
                                                                        : 8)

// IRQ Mapping with Peripherals
#define EXTI0 6
#define EXTI1 7
#define EXTI2 8
#define EXTI3 9
#define EXTI4 10
#define EXTI9_5 23
#define EXTI15_10 40

/* *******************************  */
/*    SPI Register definations      */
/*  ******************************* */

typedef struct
{
    volatile uint32_t CR1;     // Control register 1
    volatile uint32_t CR2;     // Control register 2
    volatile uint32_t SR;      // status register
    volatile uint32_t DR;      // data register
    volatile uint32_t CRCPR;   // CRC polynomial register
    volatile uint32_t RXCRCR;  // RX CRC register
    volatile uint32_t TXCRCR;  // TX CRC register
    volatile uint32_t I2SCFGR; // I2S configuration register
    volatile uint32_t I2SPR;   // I2S prescaler register
} SPI_RegDef_t;

#define SPI1 ((SPI_RegDef_t *)SPI1_BASEADDR)
#define SPI2 ((SPI_RegDef_t *)SPI2_BASEADDR)
#define SPI3 ((SPI_RegDef_t *)SPI3_BASEADDR)

/* *******************************  */
/*    USART Register definations      */
/*  ******************************* */

typedef struct
{
    volatile uint32_t SR;   //  status register
    volatile uint32_t DR;   //  data register
    volatile uint32_t BRR;  //  baud rate register
    volatile uint32_t CR1;  //  control register 1
    volatile uint32_t CR2;  //  control register 2
    volatile uint32_t CR3;  //  control register 3
    volatile uint32_t GTPR; //  Guard time and prescaler register
} USART_RegDef_t;

#define USART1 ((USART_RegDef_t *)USART1_BASEADDR)
#define USART2 ((USART_RegDef_t *)USART2_BASEADDR)
#define USART3 ((USART_RegDef_t *)USART3_BASEADDR)
#define UART4 ((USART_RegDef_t *)UART4_BASEADDR)
#define UART5 ((USART_RegDef_t *)UART5_BASEADDR)
#define USART6 ((USART_RegDef_t *)USART6_BASEADDR)
#define UART7 ((USART_RegDef_t *)UART7_BASEADDR)
#define UART8 ((USART_RegDef_t *)UART8_BASEADDR)

/* MCU Clocks */
#define CLOCK_HSI (16 * 1000000)
#define CLOCK_HSE CLOCK_HSI

/* MCU clock functions */
uint32_t Get_Peripheral_Clock(uint8_t is_apb1_clk);
uint32_t Get_Sys_Clock();
uint32_t Get_PLL_Clock();

/* MCU clock macros */
#define MHz 1000000
#define KHz 1000
#define GHz 1000000000

/* *******************************  */
/*    I2C Register definations      */
/*  ******************************* */

typedef struct
{
    volatile uint32_t CR1;   //  Control register 1
    volatile uint32_t CR2;   //  Control register 2
    volatile uint32_t OAR1;  //  Own address register 1
    volatile uint32_t OAR2;  //  Own address register 2
    volatile uint32_t DR;    //  data register
    volatile uint32_t SR1;   // status register 1
    volatile uint32_t SR2;   // status register 2
    volatile uint32_t CCR;   // clock control register
    volatile uint32_t TRISE; // Rise time register
    volatile uint32_t FLTR;  // FLTR register

} I2C_RegDef_t;

// I2C Addresses
#define I2C1 ((I2C_RegDef_t *)I2C1_BASEADDR)
#define I2C2 ((I2C_RegDef_t *)I2C2_BASEADDR)
#define I2C3 ((I2C_RegDef_t *)I2C3_BASEADDR)



/*********************************/
/* Basic Timer Register definations   */
/*********************************/

typedef struct{
    volatile uint32_t CR1;   // Control register 1
    volatile uint32_t CR2;   // Control register 2
    volatile uint32_t reserved1;  // Reserved
    volatile uint32_t DIER;  // DMA/Interrupt enable register
    volatile uint32_t SR;    // Status register
    volatile uint32_t EGR;   // Event generation register
    volatile uint32_t reserved2;  // Reserved
    volatile uint32_t reserved3;  // Reserved
    volatile uint32_t reserved4;  // Reserved
    volatile uint32_t CNT;   // Counter
    volatile uint32_t PSC;   // Prescaler
    volatile uint32_t ARR;   // Auto-reload register
} BTIM_RegDef_t;


// Timer Addresses
#define TIM6 ((BTIM_RegDef_t *)TIM6_BASEADDR)
#define TIM7 ((BTIM_RegDef_t *)TIM7_BASEADDR)



#endif