/*
 * ECE 153B
 *
 * Name(s): Andy Chen, Karl Tizon
 * Section: Monday 6PM
 * Project
 */


#include "UART.h"
#include "DMA.h"
#include <string.h>
#include <stdio.h>

static volatile DMA_Channel_TypeDef * tx;
static volatile DMA_Channel_TypeDef * tx1;
static volatile char inputs[IO_SIZE];
static volatile uint8_t data_t_0[IO_SIZE];
static volatile uint8_t data_t_1[IO_SIZE];
static volatile uint8_t input_size = 0;
static volatile uint8_t pending_size = 0;
static volatile uint8_t * active = data_t_0;
static volatile uint8_t * pending = data_t_1;

#define SEL_0 1
#define BUF_0_EMPTY 2
#define BUF_1_EMPTY 4
#define BUF_0_PENDING 8
#define BUF_1_PENDING 16

void transfer_data(char ch);
void on_complete_transfer(void);

void UART1_Init(void) {
	// Enable USART1 clock in the peripheral register
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN; 
	
	// Select system clock  as the USART1 clock source in the peripheral independent clock register
	RCC->CCIPR &= ~RCC_CCIPR_USART1SEL;
	RCC->CCIPR |= RCC_CCIPR_USART1SEL_0; 
	
	tx1 = DMA1_Channel4;
  DMA_Init_UARTx(DMA1_Channel4, USART1);
}

void UART2_Init(void) {
	// Enable USART2 clock in the peripheral register
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;
	
	// Select system clock  as the USART2 clock source in the peripheral independent clock register
	RCC->CCIPR &= ~RCC_CCIPR_USART2SEL;
	RCC->CCIPR |= RCC_CCIPR_USART2SEL_0; 
	
	tx = DMA1_Channel7;
	DMA_Init_UARTx(DMA1_Channel7, USART2);
}

void UART1_GPIO_Init(void) {
	// Configure PB6 and PB7 to operate as UART transmitters and recievers
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	
	GPIOB->MODER &= ~(GPIO_MODER_MODE6 | GPIO_MODER_MODE7);
	GPIOB->MODER |= (GPIO_MODER_MODE6_1 | GPIO_MODER_MODE7_1);
	
	// Both pins should operate at very high speed
	GPIOB->OSPEEDR |= (GPIO_OSPEEDR_OSPEED6 | GPIO_OSPEEDR_OSPEED7);
	
	// Both pins should have a push-pull output type
	GPIOB->OTYPER &= ~(GPIO_OTYPER_OT6 | GPIO_OTYPER_OT7);
	
	// Both pins should use a pull-up resistors for I/O
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD6 | GPIO_PUPDR_PUPD7);
	GPIOB->PUPDR |= (GPIO_PUPDR_PUPD6_0 | GPIO_PUPDR_PUPD7_0);
	
	GPIOB->AFR[0] &= ~(GPIO_AFRL_AFSEL6 | GPIO_AFRL_AFSEL7);
	GPIOB->AFR[0] |=  (GPIO_AFRL_AFSEL6_0 | GPIO_AFRL_AFSEL6_1 | GPIO_AFRL_AFSEL6_2
									 | GPIO_AFRL_AFSEL7_0 | GPIO_AFRL_AFSEL7_1 | GPIO_AFRL_AFSEL7_2);
}

void UART2_GPIO_Init(void) {
	// Configure PA2 and PA3 to operate as UART transmitters and recievers
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	
	GPIOA->MODER &= ~(GPIO_MODER_MODE2 | GPIO_MODER_MODE3);
	GPIOA->MODER |= (GPIO_MODER_MODE2_1 | GPIO_MODER_MODE3_1);
	
	// Both pins should operate at very high speed
	GPIOA->OSPEEDR |= (GPIO_OSPEEDR_OSPEED2 | GPIO_OSPEEDR_OSPEED3);
	
	// Both pins should have a push-pull output type
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT2 | GPIO_OTYPER_OT3);
	
	// Both pins should use a pull-up resistors for I/O
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD2 | GPIO_PUPDR_PUPD3);
	GPIOA->PUPDR |= (GPIO_PUPDR_PUPD2_0 | GPIO_PUPDR_PUPD3_0);
	
	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL2 | GPIO_AFRL_AFSEL3);
	GPIOA->AFR[0] |=  (GPIO_AFRL_AFSEL2_0 | GPIO_AFRL_AFSEL2_1 | GPIO_AFRL_AFSEL2_2
									 | GPIO_AFRL_AFSEL3_0 | GPIO_AFRL_AFSEL3_1 | GPIO_AFRL_AFSEL3_2);
}

void USART_Init(USART_TypeDef* USARTx) {
	// Disable USART to modify settings
	USARTx->CR1 &= ~USART_CR1_UE;
	
	// In the control registers, set word length to 8 bits, oversampling mode to oversample by 16, and number of stop bits to 1
	USARTx->CR1 &= ~(USART_CR1_M0 | USART_CR1_M1);
	USARTx->CR1 &= ~USART_CR1_OVER8;
	USARTx->CR2 &= ~USART_CR2_STOP;
	
	// Set baud rate to 9600
	// BRR = fCLK / baud
	USARTx->BRR = 8333; 
	
	// In the control registers, enable both the transmitter and reciever
	USARTx->CR1 |= (USART_CR1_TE | USART_CR1_RE);
	
	// Enable USART in the control registers
	USARTx->CR1 |= USART_CR1_UE;
}

/**
 * This function accepts a string that should be sent through UART
*/
void UART1_print(char* data) {
    while (*data) {
        while (!(USART1->ISR & USART_ISR_TXE));
        USART1->TDR = (uint8_t)*data++;
    }
}

void UART_print(char* data) {
		UART1_print(data);

    uint32_t len = strlen(data);
    if (len == 0) return;
    
    if (!(tx->CCR & DMA_CCR_EN)) {
        memcpy((void*)active, data, len);
        
        tx->CCR &= ~DMA_CCR_EN;
        tx->CCR &= ~DMA_CCR_TCIE; 
        tx->CMAR = (uint32_t)active;
        tx->CNDTR = len;
        DMA1->IFCR |= DMA_IFCR_CGIF7;
        USART2->ICR |= USART_ICR_TCCF;
        tx->CCR |= DMA_CCR_TCIE;    
        tx->CCR |= DMA_CCR_EN;        
    } else {
        memcpy((void*)pending, data, len);
        pending_size = (uint8_t)len;
    }
}

/**
 * This function should be invoked when a character is accepted through UART
*/
void transfer_data(char ch) {
	// Append character to input buffer.
	if (input_size < IO_SIZE - 1) {
		inputs[input_size++] = ch;
	}
	// If the character is end-of-line, invoke UART_onInput
	if (ch == '\n') {
		inputs[input_size] = '\0';
		UART_onInput((char*)inputs, input_size);
		input_size = 0;
	}
}

/**
 * This function should be invoked when DMA transaction is completed
*/
void on_complete_transfer(void) {
    if (pending_size > 0) {
        volatile uint8_t * tmp = active;
        active = pending;
        pending = tmp;
        
        uint8_t len = pending_size;
        pending_size = 0;
        
        tx->CCR &= ~DMA_CCR_EN;
        tx->CCR &= ~DMA_CCR_TCIE;
        tx->CMAR = (uint32_t)active;
        tx->CNDTR = len;
        DMA1->IFCR |= DMA_IFCR_CGIF7;
        USART2->ICR |= USART_ICR_TCCF;
        tx->CCR |= DMA_CCR_TCIE;
        tx->CCR |= DMA_CCR_EN;
    }
}

void USART1_IRQHandler(void){
	// When receive a character, invoke transfer_data
	if (USART1->ISR & USART_ISR_RXNE) {
		transfer_data((char)(USART1->RDR & 0xFF));
	}
	// When complete sending data, invoke on_complete_transfer
	if (USART1->ISR & USART_ISR_TC) {
		USART1->ICR |= USART_ICR_TCCF;
		on_complete_transfer();
	}
}

void USART2_IRQHandler(void){
    if (USART2->ISR & USART_ISR_RXNE) {
        transfer_data((char)(USART2->RDR & 0xFF));
    }
    if (USART2->ISR & USART_ISR_TC) {
        USART2->ICR |= USART_ICR_TCCF;
    }
}
