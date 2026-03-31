#ifndef TIMER_H
#define TIMER_H

#include "cstm32f407xx.h"


/* Timer functions */
void Init_basic_timer(BTIM_RegDef_t *TIMx,uint16_t prescaler,uint16_t arr);
void Start_basic_timer_IT(BTIM_RegDef_t *TIMx);
void Stop_basic_timer_IT(BTIM_RegDef_t *TIMx);
void Timer_update_event_callback();
uint8_t Get_Timer_update_status(BTIM_RegDef_t *TIMx);
void Start_basic_timer(BTIM_RegDef_t *TIMx);
void Stop_basic_timer(BTIM_RegDef_t *TIMx);

#endif