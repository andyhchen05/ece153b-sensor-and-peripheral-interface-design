#include "PWM.h"

void PWM_Init(void) {
    RCC->AHB2ENR  |= RCC_AHB2ENR_GPIOAEN;
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;

    GPIOA->MODER  &= ~GPIO_MODER_MODE5;
    GPIOA->MODER  |=  GPIO_MODER_MODE5_1;   
    GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL5;
    GPIOA->AFR[0] |=  GPIO_AFRL_AFSEL5_0; 

    TIM2->PSC  = 0;          
    TIM2->ARR  = 4095;      
    TIM2->CCR1 = 0;         

    TIM2->CCMR1 &= ~TIM_CCMR1_OC1M;
    TIM2->CCMR1 |=  TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;  
    TIM2->CCMR1 |=  TIM_CCMR1_OC1PE;   

    TIM2->CCER  |=  TIM_CCER_CC1E;
    TIM2->CCER  &= ~TIM_CCER_CC1P;    

    TIM2->CR1   |=  TIM_CR1_ARPE;
    TIM2->CR1   |=  TIM_CR1_CEN;
}

void PWM_Set_Duty(uint32_t adcValue) {
    if (adcValue > 4096) adcValue = 4096;

    TIM2->CCR1 = (adcValue == 4096) ? 4095 : adcValue;
}
