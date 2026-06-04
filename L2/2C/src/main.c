/*
 * ECE 153B
 *
 * Name(s): Andy Chen, Karl Tizon
 * Section: Monday 6PM
 * Lab: 2C
 */
 
#include "stm32l476xx.h"

#include "LED.h"
#include "RTC.h"
#include "SysClock.h"

char strTime[12] = {0};
char strDate[12] = {0};

void RTC_Set_Alarm(void) {
	RTC->CR &= ~(RTC_CR_ALRAE | RTC_CR_ALRBE);
	
	RTC_Disable_Write_Protection();
	
	while (!(RTC->ISR & RTC_ISR_ALRAWF));
	while (!(RTC->ISR & RTC_ISR_ALRBWF));
	
	// Alarm A
	RTC->ALRMAR = 0;
	RTC->ALRMAR |= (0<<4);
	RTC->ALRMAR |= (5<<0);
	RTC->ALRMAR |= RTC_ALRMAR_MSK4;
	RTC->ALRMAR |= RTC_ALRMAR_MSK3;
	RTC->ALRMAR |= RTC_ALRMAR_MSK2;
	
	// Alarm B
	RTC->ALRMBR = 0;
	RTC->ALRMBR |= RTC_ALRMBR_MSK1;
	RTC->ALRMBR |= RTC_ALRMBR_MSK2;
	RTC->ALRMBR |= RTC_ALRMBR_MSK3;
	RTC->ALRMBR |= RTC_ALRMBR_MSK4;
	
	// Enable Interrupts
	RTC->CR |= RTC_CR_ALRAIE;
	RTC->CR |= RTC_CR_ALRBIE;
	RTC->CR |= RTC_CR_ALRAE;
	//RTC->CR |= RTC_CR_ALRBE;
	
	RTC_Enable_Write_Protection();
}

void RTC_Alarm_Enable(void) {
	EXTI->RTSR1 |= EXTI_RTSR1_RT18;
	
	EXTI->IMR1 |= EXTI_IMR1_IM18;
	EXTI->EMR1 |= EXTI_EMR1_EM18;
	
	EXTI->PR1 |= EXTI_PR1_PIF18;
	
	NVIC_EnableIRQ(RTC_Alarm_IRQn);
	NVIC_SetPriority(RTC_Alarm_IRQn, 0);
}

void RTC_Alarm_IRQHandler(void) {
	if (RTC->ISR & RTC_ISR_ALRAF) {
		RTC->ISR &= ~RTC_ISR_ALRAF;
		Green_LED_Toggle();
	}
	
	if (RTC->ISR & RTC_ISR_ALRBF) {
		RTC->ISR &= ~RTC_ISR_ALRBF;
		Green_LED_Toggle();
	}
		
	EXTI->PR1 |= EXTI_PR1_PIF18;
}

int main(void) {	
	System_Clock_Init(); // Switch System Clock = 80 MHz
	
	LED_Init();
	
	RTC_Init();
	RTC_Alarm_Enable();
	RTC_Set_Alarm();
		
	while(1) {
		Get_RTC_Calendar(strTime, strDate);
	}
}
