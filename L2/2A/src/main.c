/*
 * ECE 153B
 *
 * Name(s): Andy Chen, Karl Tizon
 * Section: Monday 6PM
 * Lab: 2A
 */

#include "stm32l476xx.h"

void GPIO_Init(void){	
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	
	GPIOA->MODER &= ~(GPIO_MODER_MODE5 | GPIO_MODER_MODE6 |
										GPIO_MODER_MODE8 | GPIO_MODER_MODE9);
	GPIOA->MODER |= (GPIO_MODER_MODE5_0 | GPIO_MODER_MODE6_0 |
									 GPIO_MODER_MODE8_0 | GPIO_MODER_MODE9_0);
	
	GPIOA->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED5 | GPIO_OSPEEDR_OSPEED6 |
                      GPIO_OSPEEDR_OSPEED8 | GPIO_OSPEEDR_OSPEED9);
  GPIOA->OSPEEDR |= (GPIO_OSPEEDR_OSPEED5_1 | GPIO_OSPEEDR_OSPEED6_1 |
                     GPIO_OSPEEDR_OSPEED8_1 | GPIO_OSPEEDR_OSPEED9_1);
	
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT5 | GPIO_OTYPER_OT6 |
                     GPIO_OTYPER_OT8 | GPIO_OTYPER_OT9);
	
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD5 | GPIO_PUPDR_PUPD6 |
										GPIO_PUPDR_PUPD8 | GPIO_PUPDR_PUPD9);
}


#define DELAY 600	// delay between steps of the sequences

void Full_Stepping_Clockwise(void){
	uint16_t steps[4] = {
		(1<<5),
		(1<<6),
		(1<<8),
		(1<<9),
	};
	for (int i=0; i<4; i++) {
		GPIOA->ODR = steps[i];
		for (int d=0; d<DELAY; d++);
	}
}

void Full_Stepping_CounterClockwise(void){
	uint16_t steps[4] = {
		(1<<9),
		(1<<8),
		(1<<6),
		(1<<5),
	};
	for (int i=0; i<4; i++) {
		GPIOA->ODR = steps[i];
		for (int d=0; d<DELAY; d++);
	}
}

void Half_Stepping_Clockwise(void){
	uint16_t steps[8] = {
		(1<<5),
		(1<<5) | (1<<6),
		(1<<6),
		(1<<6) | (1<<8),
		(1<<8),
		(1<<8) | (1<<9),
		(1<<9),
		(1<<9) | (1<<5),
	};
	for (int i=0; i<8; i++) {
		GPIOA->ODR = steps[i];
		for (int d=0; d<DELAY; d++);
	}
}

void Half_Stepping_CounterClockwise(void){
	uint16_t steps[8] = {
		(1<<9) | (1<<5),
		(1<<9),
		(1<<8) | (1<<9),
		(1<<8),
		(1<<6) | (1<<8),
		(1<<6),
		(1<<5) | (1<<9),
		(1<<5)
	};
	for (int i=0; i<8; i++) {
		GPIOA->ODR = steps[i];
		for (int d=0; d<DELAY; d++);
	}	
}


int main(void){
	GPIO_Init();
	
	// Rotate 360 degrees either clockwise or counter-clockwise
	while(1) {
		//Full_Stepping_Clockwise();
		//Full_Stepping_CounterClockwise();
		Half_Stepping_Clockwise();
		//Half_Stepping_CounterClockwise();
	}
}
