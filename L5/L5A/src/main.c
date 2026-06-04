/*
 * ECE 153B
 *
 * Name(s): Andy Chen, Karl Tizon
 * Section: Monday 6PM
 * Lab: 5A
 */


#include "ADC.h"
#include "PWM.h"
#include "SysClock.h"
#include "stm32l476xx.h"
#include <stdio.h>

uint32_t adc_result = 0;
float voltage = 0.0f;
	

int main(void) {
    // Initialization
    System_Clock_Init(); // Switch System Clock = 16 MHz

    ADC_Init();

    PWM_Init();
	
    while (1) {
        // Trigger ADC and get result
				ADC1->CR |= ADC_CR_ADSTART;
				while (!(ADC123_COMMON->CSR & ADC_CSR_EOC_MST));
			
        // LED behavior based on ADC result
				adc_result = ADC1->DR;
				voltage = (adc_result / 4096.0f) * 3.3f;
				(void)voltage;
				PWM_Set_Duty(adc_result);
    }
}
