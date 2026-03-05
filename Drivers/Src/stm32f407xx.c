#include "stm32f407xx.h"

/*
 *   Helper function
 *   get PLL clock
 *   need to implement
 */

uint32_t Get_PLL_Clock()
{
    return CLOCK_HSI;
}

/*
 *   Helper function
 *   get System clock prescaled by ahb prescaler
 */

uint32_t Get_Sys_Clock()
{
    uint8_t clk = (RCC->CFGR >> 2) & 0x3;
    uint32_t sysclk = 0;

    uint16_t ahb_pre[] = {2, 4, 8, 16, 64, 128, 256, 512};

    switch (clk)
    {
    case 0:
        sysclk = CLOCK_HSI; // HSI
        break;
    case 1:
        sysclk = CLOCK_HSE; // HSE
        break;

    case 2:
        sysclk = Get_PLL_Clock(); // PLL
        break;

    default:
        break;
    }

    uint8_t hpse = ((RCC->CFGR >> 4) && 0xF);
    if (hpse < 8)
    {
        return sysclk;
    }
    return (sysclk / ahb_pre[hpse - 8]);
}

/*
 *   Helper function
 *   get Peripheral clock
 */
uint32_t Get_Peripheral_Clock(uint8_t is_apb1_clk)
{
    uint32_t sysclk = Get_Sys_Clock();
    uint8_t apb_pre = 0;
    uint8_t apb_prescaler[] = {2, 4, 8, 16};
    if (is_apb1_clk)
    {
        apb_pre = (RCC->CFGR >> 10) & 0x7;
    }
    else
    {
        apb_pre = (RCC->CFGR >> 13) & 0x7;
    }
    if (apb_pre < 4)
        return sysclk;

    return sysclk / (apb_prescaler[apb_pre - 4]);
}