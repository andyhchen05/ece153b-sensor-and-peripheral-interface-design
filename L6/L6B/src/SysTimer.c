/*
 * ECE 153B
 *
 * Name(s): Andy Chen, Karl Tizon
 * Section: Monday 6PM
 * Lab: 6B
 */

#include "SysTimer.h"

static uint32_t volatile step;

void SysTick_Init(void) {
	// SysTick Control & Status Register
	SysTick->CTRL = 0; // Disable SysTick IRQ and SysTick Counter
	
	// Enables SysTick exception request
	// 1 = counting down to zero asserts the SysTick exception request
	// 0 = counting down to zero does not assert the SysTick exception request
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	
	// Select clock source
	// If CLKSOURCE = 0, the external clock is used. The frequency of SysTick clock is the frequency of the AHB clock divided by 8.
	// If CLKSOURCE = 1, the processor clock is used.
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
	
	// Configure and Enable SysTick interrupt in NVIC
	NVIC_EnableIRQ(SysTick_IRQn);
	NVIC_SetPriority(SysTick_IRQn, 1); // Set Priority to 1
}

void SysTick_Handler(void) {
	++step;
}

void delay(uint32_t ms) {
	// Reset the counter
	step = 0;
	
	// Reset VAL to 0 for SysTick
	SysTick->VAL = 0;
	
	// Set LOAD to a desirable value for SysTick
	SysTick->LOAD = 80000U - 1U;
	
	// Enable SysTick
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	
	// Busy waiting for counter to reach desired value
	while (step < ms) {}
	
	// Disable SysTick for future use
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void startTimer(void) {
	// Reset the counter
	step = 0;
	
	// Reset VAL to 0 for SysTick
	SysTick->VAL = 0;
	
	// Set LOAD to a desirable value for SysTick
	SysTick->LOAD =  80000U - 1U;
	
	// Enable SysTick to start timing
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

uint32_t endTimer(void) {
	// Disable SysTick first to pause the timer
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	
	// Read the values from VAL and the counter
	uint32_t val = SysTick->VAL;
	
	// Calculate the time using both numbers
	uint32_t totalTicks = (step * 80000U) + (80000U  - val);
	uint32_t elapsed_us = totalTicks / 80U;
	
	return elapsed_us;
}
