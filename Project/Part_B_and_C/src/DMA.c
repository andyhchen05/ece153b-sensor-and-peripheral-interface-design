/*
 * ECE 153B
 *
 * Name(s): Andy Chen, Karl Tizon
 * Section: Monday 6PM
 * Project
 */
 
#include "DMA.h"
#include "SysTimer.h"

#include "DMA.h"

void DMA_Init_UARTx(DMA_Channel_TypeDef * tx, USART_TypeDef * uart) {
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
    volatile uint32_t dummy = RCC->AHB1ENR; (void)dummy;

    tx->CCR &= ~DMA_CCR_EN;

    if (tx == DMA1_Channel7) {
        DMA1->IFCR |= DMA_IFCR_CGIF7;
        DMA1_CSELR->CSELR &= ~DMA_CSELR_C7S;
        DMA1_CSELR->CSELR |= (0x2 << 24);
        NVIC_SetPriority(DMA1_Channel7_IRQn, 2);
        NVIC_EnableIRQ(DMA1_Channel7_IRQn);
    } else if (tx == DMA1_Channel4) {
    DMA1->IFCR |= DMA_IFCR_CGIF4;
    DMA1_CSELR->CSELR &= ~DMA_CSELR_C4S;
    DMA1_CSELR->CSELR |= (0x2 << 12);
    NVIC_SetPriority(DMA1_Channel4_IRQn, 2);
    NVIC_EnableIRQ(DMA1_Channel4_IRQn);
		}

    tx->CPAR = (uint32_t)(&uart->TDR);
    tx->CCR &= ~DMA_CCR_MEM2MEM;
    tx->CCR &= ~DMA_CCR_PL;
    tx->CCR |= DMA_CCR_PL_1;
    tx->CCR &= ~DMA_CCR_MSIZE;
    tx->CCR &= ~DMA_CCR_PSIZE;
    tx->CCR |= DMA_CCR_MINC;
    tx->CCR &= ~DMA_CCR_PINC;
    tx->CCR &= ~DMA_CCR_CIRC;
    tx->CCR |= DMA_CCR_DIR;
		tx->CCR |= DMA_CCR_TCIE;
    tx->CCR &= ~(DMA_CCR_HTIE | DMA_CCR_TEIE);
}

void DMA1_Channel7_IRQHandler(void) {
    if (DMA1->ISR & DMA_ISR_TCIF7) {
        DMA1->IFCR |= DMA_IFCR_CTCIF7;
        DMA1_Channel7->CCR &= ~DMA_CCR_EN;
    }
}

void DMA1_Channel4_IRQHandler(void) {
	if (DMA1->ISR & DMA_ISR_TCIF4) {
		DMA1->IFCR |= DMA_IFCR_CTCIF4;
		DMA1_Channel4->CCR &= ~DMA_CCR_EN;
	}
}