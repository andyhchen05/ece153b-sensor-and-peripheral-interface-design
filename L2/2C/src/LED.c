/*
 * ECE 153B
 *
 * Name(s): Andy Chen, Karl Tizon
 * Section: Monday 6PM
 * Lab: 2C
 */

#include "LED.h"

void LED_Init(void) {
	// Enable GPIO Clocks
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	
	// Initialize Green LED
	GPIOA->MODER &= ~GPIO_MODER_MODE5;
	GPIOA->MODER |= GPIO_MODER_MODE5_0;
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT5;
	GPIOA->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED5;
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED5_1;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD5;
	GPIOA->ODR &= ~GPIO_ODR_OD5;
}

void Green_LED_Off(void) {
	GPIOA->ODR &= ~GPIO_ODR_OD5;
}

void Green_LED_On(void) {
	GPIOA->ODR |= GPIO_ODR_OD5;
}

void Green_LED_Toggle(void) {
	GPIOA->ODR ^= GPIO_ODR_OD5;
}
