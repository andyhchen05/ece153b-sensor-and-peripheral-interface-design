/*
 * ECE 153B
 *
 * Name(s): Andy Chen, Karl Tizon
 * Section: Monday 6PM
 * Lab: 2B
 */

#include "EXTI.h"
#include "LED.h"

void EXTI_Init(void) {
	// Initialize User Button
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	GPIOC->MODER &= ~GPIO_MODER_MODE13;
	GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD13;
	
	// Configure SYSCFG EXTI
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13;
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;
	
	// Configure EXTI Trigger
	EXTI->RTSR1 &= ~EXTI_RTSR1_RT13;
	EXTI->FTSR1 |= EXTI_FTSR1_FT13;
	
	// Enable EXTI
	EXTI->IMR1 |= EXTI_IMR1_IM13;
	
	// Configure and Enable in NVIC
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	NVIC_SetPriority(EXTI15_10_IRQn, 0);
}

// [TODO] Write Interrupt Handlers (look in startup_stm32l476xx.s to find the 
// interrupt handler names that you should use)
int count = 0;
void EXTI15_10_IRQHandler(void) {
	if (EXTI->PR1 & EXTI_PR1_PIF13) {
		EXTI->PR1 |= EXTI_PR1_PIF13;
		count++;
		
		if (count % 2 == 1) {
			GPIOA->ODR |= (1<<5);
		} else {
			GPIOA->ODR &= ~(1<<5);
		}
	}
}

