/*
 * ECE 153B
 *
 * Name(s): Andy Chen, Karl Tizon
 * Section: Monday 6PM
 * Project
 */

#include "stm32l476xx.h"
#include "SysClock.h"
#include "SysTimer.h"
#include "UART.h"
#include "motor.h"
#include <stdio.h>

int main(void) {
	// Switch System Clock = 80 MHz
	System_Clock_Init(); 
	Motor_Init();
	SysTick_Init();
	UART2_GPIO_Init();
	UART2_Init();
	USART_Init(USART2);
	
	printf("Program Starts.\r\n");
	printf("Commands: 'o' = open (CW), 'c' = close (CCW), 's' = stop\r\n");
	while(1) {
		if (USART2->ISR & USART_ISR_RXNE) {
			char c = (char)(USART2->RDR & 0xFF);
			
			if (c == 'o' || c == 'O') setDire(1);
			else if (c == 'c' | c == 'C') setDire(-1);
			else if (c == 's' | c == 'S') setDire(0);
			
			while (!(USART2->ISR & USART_ISR_TXE));
			USART2->TDR = c;
		}
	}
}


