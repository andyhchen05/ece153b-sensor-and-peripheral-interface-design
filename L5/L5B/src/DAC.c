#include "stm32l476xx.h"

static void DAC_Pin_Init(void) {
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
		GPIOA->MODER |= GPIO_MODER_MODE4;
		GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD4;
}

void DAC_Write_Value(uint32_t value) {
    // Write DAC1Ch1 right-aligned 12-bit value
		DAC1->DHR12R1 = value & 0xFFF;
}

void DAC_Init(void) {
    DAC_Pin_Init();

    // Enable DAC clock
		RCC->APB1ENR1 |= RCC_APB1ENR1_DAC1EN;
	
		// Before modifying any of its settings, ensure that it is disabled
		DAC1->CR &= ~DAC_CR_EN1;
	
    // Enable software trigger mode
		DAC1->CR &= ~DAC_CR_EN1;
		DAC1->CR |= DAC_CR_TSEL1_0 | DAC_CR_TSEL1_1 | DAC_CR_TSEL1_2;

    // Disable trigger
		DAC1->CR &= ~DAC_CR_TEN1;

    // DAC1 connected in normal mode to external pin only with buffer enabled
		DAC1->MCR &= ~DAC_MCR_MODE1;

    // Enable DAC channel 1
		DAC1->CR |= DAC_CR_EN1;

    DAC_Write_Value(0);
}
