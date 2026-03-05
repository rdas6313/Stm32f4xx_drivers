#include "stm32f4xx_timer.h"

/*
* @brief This is a static function that checks the validity of the timer pointer passed to it. It verifies whether the pointer corresponds to either TIM6 or TIM7, which are the basic timers available in the STM32F4xx series. If the pointer is valid, it returns 1; otherwise, it returns 0.
* @param TIMx: Pointer to the timer peripheral (TIM6 or TIM7)
* @return 1 if the timer pointer is valid, 0 otherwise
*/
static uint8_t check_timer_validity(BTIM_RegDef_t *TIMx){
    if(TIMx == TIM6 || TIMx == TIM7)
        return 1; // Valid timer pointer
    return 0; // Invalid timer pointer
}

/*
* @brief This function initializes a basic timer (TIM6 or TIM7) with the specified prescaler and auto-reload values. It also enables the clock for the timer peripheral.
* @param TIMx: Pointer to the timer peripheral (TIM6 or TIM7)
* @param prescaler: The value to be loaded into the timer's prescaler register (
* this value determines the timer's counting frequency)
* @param arr: The value to be loaded into the timer's auto-reload register (this value determines the timer's period)
* @return None
* @Note The actual timer frequency can be calculated using the formula: Timer Frequency = (Timer Clock Frequency) / ((Prescaler + 1) * (Auto-reload Value + 1
*/
void Init_basic_timer(BTIM_RegDef_t *TIMx,uint16_t prescaler,uint16_t arr){
    // 1. Enable clock for the timer peripheral
    if(TIMx == TIM6){
        TIM6_PCLK_EN();
    }else if(TIMx == TIM7){
        TIM7_PCLK_EN();
    }else
        return; // Invalid timer pointer, do nothing

    // 2. Configure the timer's prescaler and auto-reload values
    TIMx->PSC = prescaler;
    TIMx->ARR = arr;
    
     // 3. Generate an update event to load the prescaler value immediately
    TIMx->EGR |= (1 << 0); // UG bit in EGR register
}

/*
* @brief This is a weakly defined callback function that can be overridden by the user application
* to handle timer update events. When the timer generates an update event (e.g., when the counter overflows), this callback function will be called. The user can provide their own implementation of this function in their application code to perform specific actions when a timer update event occurs.
* @param None
* @return None
*/
__attribute__((weak)) 
void Timer_update_event_callback(){
    // This is a weakly defined callback function that can be overridden by the user application to handle timer update events.
}

/*
* @brief This function handles the timer update interrupt by checking the update interrupt flag (UIF) in the status register (SR) of the timer. If the UIF flag is set, it clears the flag and calls the user-defined callback function to handle the timer event.
* @param TIMx: Pointer to the timer peripheral (TIM6 or TIM7)
* @return None
*/
void Basic_Timer_IRQHandling(BTIM_RegDef_t *TIMx){
    if(!check_timer_validity(TIMx))
        return; // Invalid timer pointer, do nothing

    // 1. Check if the update interrupt flag (UIF) is set in the status register (SR)
    if(TIMx->SR & (1 << 0)){
        // 2. Clear the update interrupt flag by writing 0 to it
        TIMx->SR &= ~(1 << 0); // Clear UIF flag

        // 3. Call the user-defined callback function to handle the timer event
        Timer_update_event_callback();
    }
}

/*
* @brief This function starts the basic timer in polling mode by starting the timer counter without enabling interrupts.
* @param TIMx: Pointer to the timer peripheral (TIM6 or TIM7)
* @return None
*/
void Start_basic_timer(BTIM_RegDef_t *TIMx){
    if(!check_timer_validity(TIMx))
        return; // Invalid timer pointer, do nothing

    TIMx->CNT = 0; // Reset the timer counter to 0
    TIMx->SR = 0; // Clear the status register to reset any pending flags
    // 1. Start the timer by setting the CEN bit in the CR1 register
    TIMx->CR1 |= (1 << 0); // Start the timer
}

/*
*   @brief This function stops the basic timer in polling mode by clearing the CEN bit in the CR1 register, which stops the timer counter.
*   @param TIMx: Pointer to the timer peripheral (TIM6 or TIM7)
*   @return None
*/
void Stop_basic_timer(BTIM_RegDef_t *TIMx){
    if(!check_timer_validity(TIMx))
        return; // Invalid timer pointer, do nothing

    // 1. Stop the timer by clearing the CEN bit in the CR1 register
    TIMx->CR1 &= ~(1 << 0); // Stop the timer
}

/*
* @brief This function checks the update interrupt flag (UIF) in the status register (SR) of the timer to determine if an update event has occurred. If the UIF flag is set, it clears the flag and returns 1 to indicate that an update event occurred. If the UIF flag is not set, it returns 0 to indicate that no update event occurred.
* @param TIMx: Pointer to the timer peripheral (TIM6 or TIM7)
* @return 1 if an update event occurred, 0 otherwise
*/
uint8_t Get_Timer_update_status(BTIM_RegDef_t *TIMx){
    if(!check_timer_validity(TIMx))
        return 0; // Invalid timer pointer, do nothing

    // 1. Check the update interrupt flag (UIF) in the status register (SR)
    if(TIMx->SR & (1 << 0)){
        // 2. Clear the update interrupt flag by writing 0 to it
        TIMx->SR &= ~(1 << 0); // Clear UIF flag
        return 1; // Update event occurred
    }
    return 0; // No update event
}

/*
* @brief This function starts the basic timer in interrupt mode by enabling the timer update interrupt and starting the timer counter.
* @param TIMx: Pointer to the timer peripheral (TIM6 or TIM7)
* @return None
*/
void Start_basic_timer_IT(BTIM_RegDef_t *TIMx){
    if(!check_timer_validity(TIMx))
        return; // Invalid timer pointer, do nothing

    // 1. Enable the timer update interrupt
    TIMx->DIER |= (1 << 0); // Enable update interrupt

    TIMx->CNT = 0; // Reset the timer counter to 0
    TIMx->SR = 0; // Clear the status register to reset any pending flags

    // 2. Start the timer by setting the CEN bit in the CR1 register
    TIMx->CR1 |= (1 << 0); // Start the timer

}


/*
* @brief This function stops the basic timer in interrupt mode by disabling the timer update interrupt and stopping the timer counter.
* @param TIMx: Pointer to the timer peripheral (TIM6 or TIM7)
* @return None
*/
void Stop_basic_timer_IT(BTIM_RegDef_t *TIMx){
    if(!check_timer_validity(TIMx))
        return; // Invalid timer pointer, do nothing
    
    // 1. Stop the timer by clearing the CEN bit in the CR1 register
    TIMx->CR1 &= ~(1 << 0); // Stop the timer

    // 2. Disable the timer update interrupt
    TIMx->DIER &= ~(1 << 0); // Disable update interrupt
}