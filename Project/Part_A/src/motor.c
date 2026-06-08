/*
 * ECE 153B
 *
 * Name(s): Andy Chen, Karl Tizon
 * Section: Monday 6PM
 * Project
 */

#include "stm32l476xx.h"
#include "motor.h"

static const uint32_t MASK = (1<<0)|(1<<1)|(1<<8)|(1<<9);

static const uint32_t HalfStep[8] = {
    (1<<0),
    (1<<0)|(1<<1),
    (1<<1),
    (1<<1)|(1<<8),
    (1<<8),
    (1<<8)|(1<<9),
    (1<<9),
    (1<<9)|(1<<0)
};

static volatile int8_t dire = 0;
static volatile uint8_t step = 0;

void Motor_Init(void) {
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

    GPIOA->MODER &= ~(GPIO_MODER_MODE0 | GPIO_MODER_MODE1 |
                      GPIO_MODER_MODE8 | GPIO_MODER_MODE9);
    GPIOA->MODER |=  (GPIO_MODER_MODE0_0 | GPIO_MODER_MODE1_0 |
                      GPIO_MODER_MODE8_0 | GPIO_MODER_MODE9_0);

    GPIOA->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED0 | GPIO_OSPEEDR_OSPEED1 |
                        GPIO_OSPEEDR_OSPEED8 | GPIO_OSPEEDR_OSPEED9);
    GPIOA->OSPEEDR |=  (GPIO_OSPEEDR_OSPEED0_1 | GPIO_OSPEEDR_OSPEED1_1 |
                        GPIO_OSPEEDR_OSPEED8_1 | GPIO_OSPEEDR_OSPEED9_1);

    GPIOA->OTYPER &= ~(GPIO_OTYPER_OT0 | GPIO_OTYPER_OT1 |
                       GPIO_OTYPER_OT8 | GPIO_OTYPER_OT9);

    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD0 | GPIO_PUPDR_PUPD1 |
                      GPIO_PUPDR_PUPD8 | GPIO_PUPDR_PUPD9);

    GPIOA->ODR &= ~MASK;
}

void rotate(void) {
    if (dire == 0) {
        GPIOA->ODR &= ~MASK;
        return;
    }
    step = (uint8_t)((step + (uint8_t)(dire == 1 ? 1 : 7)) % 8);
    GPIOA->ODR = (GPIOA->ODR & ~MASK) | HalfStep[step];
}

void setDire(int8_t direction) {
    dire = direction;
    if (direction == 0) {
        GPIOA->ODR &= ~MASK;
    }
}


