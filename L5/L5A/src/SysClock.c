#include "SysClock.h"

void System_Clock_Init(void) {
	// Enable MSI
	RCC->CR |= RCC_CR_MSION;
	while((RCC->CR & RCC_CR_MSIRDY) == 0);
	
	// Set MSI to 8 MHz
	RCC->CR &= ~RCC_CR_MSIRANGE;
	RCC->CR |= RCC_CR_MSIRANGE_6;
	RCC->CR |= RCC_CR_MSIRGSEL;
	
	// Select HSI as system clock source
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_MSI;
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_MSI);
}
