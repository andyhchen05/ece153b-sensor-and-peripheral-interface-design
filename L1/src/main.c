/*
 * ECE 153B
 *
 * Name(s): Andy Chen, Karl Tizon
 * Section: Monday 6PM
 * Lab: 1
 */
#include "stm32l476xx.h"

void Init(){
    // Enable HSI
    RCC->CR |= ((uint32_t)RCC_CR_HSION);
    while ( (RCC->CR & (uint32_t) RCC_CR_HSIRDY) == 0 );

    // Select HSI as system clock source
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_HSI;
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) == 0 );

    // Enable GPIO Clock
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	
    // Initialize Green LED
    GPIOA->MODER &= ~GPIO_MODER_MODE5;
		GPIOA->MODER |= GPIO_MODER_MODE5_0;
		GPIOA->OTYPER &= ~GPIO_OTYPER_OT5;
	  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD5;

    // Initialize User Button
		GPIOC->MODER &= ~GPIO_MODER_MODE13;
		GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD13;
}

int main(void){
    // Initialization
    Init();

    // Polling to Check for User Button Presses
		int prev = 1;  

    while (1) {
        int curr = (GPIOC->IDR & GPIO_IDR_ID13) ? 1 : 0;
        if (prev == 1 && curr == 0) {
            GPIOA->ODR ^= GPIO_ODR_OD5; 
        }
        prev = curr; 
    }
    return 0;
}
