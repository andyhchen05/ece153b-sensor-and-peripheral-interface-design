/*
 * ECE 153B
 *
 * Name(s): Andy Chen, Karl Tizon
 * Section: Monday 6PM
 * Lab: 3C
 */
 
#include <stdio.h> 
 
#include "stm32l476xx.h"

uint32_t volatile currentValue = 0;
uint32_t volatile lastValue = 0;
uint32_t volatile overflowCount = 0;
uint32_t volatile timeInterval = 0;
volatile int edge = 0;
volatile uint32_t distance_cm = 0;

void Input_Capture_Setup() {
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

	GPIOB->MODER &= ~GPIO_MODER_MODE6;
	GPIOB->MODER |= GPIO_MODER_MODE6_1;
	
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD6;
	
	GPIOB->AFR[0] &= ~(0xF << (4*6));
	GPIOB->AFR[0] |=  (2 << (4*6));
	
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM4EN;
	
	TIM4->PSC = 15;
	
	TIM4->ARR = 0xFFFF;
	TIM4->CR1 |= TIM_CR1_ARPE;
	
	TIM4->CCMR1 &= ~TIM_CCMR1_CC1S;
	TIM4->CCMR1 |= TIM_CCMR1_CC1S_0;
	
  TIM4->CCER |= TIM_CCER_CC1E;  
  TIM4->CCER |= TIM_CCER_CC1P;   
  TIM4->CCER |= TIM_CCER_CC1NP;  
	
	TIM4->DIER |= TIM_DIER_CC1IE | TIM_DIER_CC1DE | TIM_DIER_UIE;
	
	TIM4->EGR |= TIM_EGR_UG;
	
	TIM4->SR &= ~TIM_SR_UIF;
	
	NVIC_EnableIRQ(TIM4_IRQn);
	NVIC_SetPriority(TIM4_IRQn, 2);
	
	TIM4->CR1 |= TIM_CR1_CEN;
}

void TIM4_IRQHandler(void) {
	if (TIM4->SR & TIM_SR_UIF) {
		overflowCount++;
		TIM4->SR &= ~TIM_SR_UIF;
		if (overflowCount > 2) {
		overflowCount = 0;
		edge = 0;
		}
	}
	
	if (TIM4->SR & TIM_SR_CC1IF) {
		currentValue = TIM4->CCR1;
		if (edge == 0) {
			// Rising Edge
			lastValue = currentValue;
			overflowCount = 0;
			edge = 1;
		} else {
			timeInterval = (currentValue - lastValue) + overflowCount * (0x10000);
			edge = 0;
		}
	}
}

void Trigger_Setup() {
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	GPIOA->MODER &= ~GPIO_MODER_MODE9;
	GPIOA->MODER |= GPIO_MODER_MODE9_1;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD9;
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT9;
	GPIOA->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED9;
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED9_1;
	
	GPIOA->AFR[1] &= ~(0xF << 4);
	GPIOA->AFR[1] |= (1 << 4);
	
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	
	TIM1->PSC = 15;
	
	TIM1->ARR = 0xFFFF;
	TIM1->CR1 |= TIM_CR1_ARPE;
	
	TIM1->CCR2 = 10;
	
	TIM1->CCMR1 &= ~TIM_CCMR1_OC2M;
	TIM1->CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1;
	
	TIM1->CCMR1 |= TIM_CCMR1_OC2PE;
	TIM1->CCER |= TIM_CCER_CC2E;
	
	TIM1->BDTR |= TIM_BDTR_MOE | TIM_BDTR_OSSR | TIM_BDTR_OSSI;
	
	TIM1->EGR |= TIM_EGR_UG;
	
	TIM1->DIER |= TIM_DIER_UIE;
	TIM1->SR &= ~TIM_SR_UIF;
	
	TIM1->CR1 |= TIM_CR1_CEN;
}

int main(void) {	
	// Enable High Speed Internal Clock (HSI = 16 MHz)
	RCC->CR |= RCC_CR_HSION;
	while ((RCC->CR & RCC_CR_HSIRDY) == 0); // Wait until HSI is ready
	
	// Select HSI as system clock source 
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_HSI;
	while ((RCC->CFGR & RCC_CFGR_SWS) == 0); // Wait until HSI is system clock source
  
	// Input Capture Setup
	Input_Capture_Setup();
	
	// Trigger Setup
	Trigger_Setup();
	
	while(1) {
		if (timeInterval == 0 || timeInterval > 38000 ) {
			distance_cm = 0;
		} else {
			distance_cm = timeInterval / 58;
		}
	}
}
