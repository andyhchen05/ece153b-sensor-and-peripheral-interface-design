/*
 * ECE 153B
 *
 * Name(s): Andy Chen, Karl Tizon
 * Section: Monday 6PM
 * Lab: 4A
 */


#include "stm32l476xx.h"
#include "LED.h"
#include "SysClock.h"
#include "UART.h"
#include <string.h>
#include <stdio.h>

// Initializes USARTx
// USART2: UART Communication with Termite
// USART1: Bluetooth Communication with Phone
void Init_USARTx(int x) {
	if(x == 1) {
		UART1_Init();
		UART1_GPIO_Init();
		USART_Init(USART1);
	} else if(x == 2) {
		UART2_Init();
		UART2_GPIO_Init();
		USART_Init(USART2);
	} else {
		// Do nothing...
	}
}

int main(void) {
	System_Clock_Init(); // Switch System Clock = 80 MHz
	LED_Init();
	
	// Initialize UART -- change the argument depending on the part you are working on
	Init_USARTx(2);
	
	char rxByte;
	while(1) {
		printf("Enter a command (Y/y to turn LED on, N/n to turn LED off): \r\n");
		scanf("%c", &rxByte);
		if (rxByte == 'Y' || rxByte == 'y') {
			Green_LED_On();
			printf("The green LED has been turned on.\r\n");
		} else if (rxByte == 'N' || rxByte == 'n') {
			Green_LED_Off();
			printf("The green LED has been turned off.\r\n");
		} else {
			printf("Unrecognized command. Try again with a valid command.\r\n");
		}
		UART1_Init();
    UART1_GPIO_Init();
    USART_Init(USART1);
	}
	
}
