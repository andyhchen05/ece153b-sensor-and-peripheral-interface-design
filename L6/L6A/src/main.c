/*
 * ECE 153B
 *
 * Name(s): Andy Chen, Karl Tizon
 * Section: Monday 6PM
 * Lab: 6A
 */

#include "stm32l476xx.h"
#include "SysClock.h"
#include "SysTimer.h"
#include "LED.h"
#include "CRC.h"
#include "UART.h"
#include <stdio.h>

/* Expected CRC Value */
#define uwExpectedCRCValue 0x5A60861E

static volatile uint32_t Software_ComputedCRC;

int main(void) {
	uint32_t elapsed_us = 0;
	
	// Switch System Clock = 80 MHz
	System_Clock_Init(); 
	// Initialize modules
	SysTick_Init();
	LED_Init();
	UART2_GPIO_Init();
	UART2_Init();
	USART_Init(USART2);
	
	while(1) {
		LED_Toggle();
		// initialize CRC
		Software_ComputedCRC = INITIAL_CRC_VALUE;
		// start timer
		startTimer();
		// compute CRC
		for (uint32_t i = LOWER; i < UPPER; i += STEP) {
			Software_ComputedCRC = CrcSoftwareFunc(Software_ComputedCRC, DataBuffer[i], POLYNOME);
		}
		// end timer
		elapsed_us = endTimer();
		// check CRC
		if (Software_ComputedCRC != uwExpectedCRCValue) {
			LED_Off();
			break;
		}
		// print time
		printf("Computation Time: %lu us\r\n", elapsed_us);
		// delay
		delay(1000);
	}
}
