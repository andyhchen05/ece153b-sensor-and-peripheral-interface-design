/*
 * ECE 153B
 * 
 * Name(s): Andy Chen, Karl Tizon
 * Section: Monday 6PM
 * Lab: 3B
 */

#include "stm32l476xx.h"

void LED_Pin_Init(void);
void TIM2_CH1_Init(void);
void SERVO_Pin_Init(void);
void TIM5_CH1_Init(void);

void LED_Pin_Init(void){
  // Enable GPIO Clocks
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	// Initialize Green LED
	// GPIO Mode
	GPIOA->MODER &= ~GPIO_MODER_MODE5;
	GPIOA->MODER |= GPIO_MODER_MODE5_1;
	// Alternative Function
	GPIOA->AFR[0] &= ~(0xF << 20);
	GPIOA->AFR[0] |= (1 << 20);
	//Set I/O output speed value as very high speed
	GPIOA->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED5;
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED5_1;
	// GPIO Push-Pull: No pull-up, no pull-down
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD5;
}
	

void TIM2_CH1_Init(void){
	// Enable the timer clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
	// Counting direction: 0 = up-counting, 1 = down-counting
	TIM2->CR1 &= ~TIM_CR1_DIR;
  // Prescaler
	TIM2->PSC = 3;
  // Auto-reload
	TIM2->ARR = 500 - 1;
	// Disable output compare mode
	TIM2->CCMR1 &= ~TIM_CCMR1_OC1M;
	// PWM mode 1
	TIM2->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
	// Output 1 preload enable
	TIM2->CCMR1 |= TIM_CCMR1_OC1PE;
	// Select output polarity: active high
	TIM2->CCER &= ~TIM_CCER_CC1P;
  // Enable output for ch1
	TIM2->CCER |= TIM_CCER_CC1E;
	// Output Compare Register for channel 1 
	TIM2->CCR1 = 250;
	// Enable counter
	TIM2->CR1 |= TIM_CR1_CEN;
}

void SERVO_Pin_Init(void){
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	// Set mode as Alternative Function 1
	GPIOA->MODER &= ~GPIO_MODER_MODE0;
	GPIOA->MODER |= GPIO_MODER_MODE0_1;
	GPIOA->AFR[0] &= ~(0xF << 0);
	GPIOA->AFR[0] |= (2 << 0);
	//Set I/O output speed value as very high speed
	GPIOA->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED0;
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED0_1;
	//Set I/O as no pull-up pull-down 
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD0;
}

void TIM5_CH1_Init(void){
	// Enable the timer clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM5EN;
	// Counting direction: 0 = up-counting, 1 = down-counting
  TIM5->CR1 &= ~TIM_CR1_DIR;
  // Prescaler
	TIM5->PSC = 3;
  // Auto-reload
	TIM5->ARR = 20000 - 1;
	// Disable output compare mode
	TIM5->CCMR1 &= ~TIM_CCMR1_OC1M;
	// PWM mode 1
	TIM5->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
	// Output 1 preload enable
	TIM5->CCMR1 |= TIM_CCMR1_OC1PE;
	// Select output polarity: active high
	TIM5->CCER &= ~TIM_CCER_CC1P;
  // Enable output for ch1
	TIM5->CCER |= TIM_CCER_CC1E;
	// Output Compare Register for channel 1 
	TIM5->CCR1 = 1500;
	// Enable counter
	TIM5->CR1 |= TIM_CR1_CEN;
}

int main(void) {
	int i;
	int led = 0;
	int dire = 1;
	
	LED_Pin_Init();
	SERVO_Pin_Init();
	TIM2_CH1_Init();
	TIM5_CH1_Init();
		
    while(1) {
		// TODO
		TIM2->CCR1 = led; 
			
		TIM5->CCR1 = 700 + (led*2000) / 499;
		
		led += dire * 10;
		if (led >= 499) {
			led = 499;
			dire = -1;
		} else if (led <= 0) {
			led = 0;
			dire = 1;
		}	
		
		for(i=0;i<3000;i++);  		// delay
    }
}