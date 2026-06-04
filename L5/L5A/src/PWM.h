#ifndef __STM32L476G_DISCOVERY_PWM_H
#define __STM32L476G_DISCOVERY_PWM_H

#include "stm32l476xx.h"

// Initialize TIM2 CH1 (PA5) for PWM output to drive LED brightness
void PWM_Init(void);

// Set PWM duty cycle: value in range [0, 4096]
void PWM_Set_Duty(uint32_t adcValue);

#endif /* __STM32L476G_DISCOVERY_PWM_H */
