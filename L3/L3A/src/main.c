/*
 * ECE 153B
 *
 * Name(s): Andy Chen, Karl Tizon
 * Section: Monday 6PM
 * Lab: 3A
 */

#include "stm32l476xx.h"
#include "LED.h"
#include "SysTimer.h"

void System_Clock_Init(void) {
	// Select MSI as system clock source
	RCC->CFGR &= ~RCC_CFGR_SW; // [TODO] - Replace with value that will make MSI the system clock source
	
	// Set MSI clock range
	RCC->CR &= ~RCC_CR_MSIRANGE;
	RCC->CR |= RCC_CR_MSIRANGE_7; // [TODO] - Replace with value that will make range 8 MHz
	
	// Use the MSI clock range that is defined in RCC_CR
	RCC->CR |= RCC_CR_MSIRGSEL; // [TODO] - Replace with value that will select range in RCC->CR
	
	// Enable MSI oscillator
	RCC->CR |= RCC_CR_MSION; // [TODO] - Replace with value that will enable MSI
	
	// Wait until MSI is ready
	while((RCC->CR & RCC_CR_MSIRDY /* [TODO] - Replace with value that checks whether MSI is ready */) == 0);
}

int main() {
	// Initialization 
	System_Clock_Init();
	SysTick_Init();
	LED_Init();
	
	while(1) {
		Green_LED_Toggle();
		delay(1000);
	}
}
