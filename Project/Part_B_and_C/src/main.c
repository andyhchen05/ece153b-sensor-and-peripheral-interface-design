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
#include "LED.h"
#include "DMA.h"
#include "UART.h"
#include "motor.h"
#include "SPI.h"
#include "I2C.h"
#include "accelerometer.h"
#include <stdio.h>
#include <string.h>

#define TEMP_OPEN_THRESHOLD 26.0
#define TEMP_CLOSE_THRESHOLD 22.0
#define CONSOLE_LOCK_MS 4000
#define MEASURE_INTERVAL_MS 200
#define DOOR_MOVE_TIMEOUT_MS 4000
#define ACC_THRESHOLD 1.0

#define TC74_ADDRESS 0x48

static volatile uint8_t cmdOpen = 0;
static volatile uint8_t cmdClose = 0;
static volatile uint8_t cmdStop = 0;

static char buffer[IO_SIZE];

typedef enum {
	DOOR_CLOSED,
	DOOR_OPEN
} DoorState;

static volatile DoorState doorState = DOOR_CLOSED;

static volatile uint32_t consoleLockRemaining = 0;
static volatile uint8_t consoleLocked = 0;

void App_SysTick_Hook(void) {
	if (consoleLockRemaining > 0) {
		consoleLockRemaining--;
		if (consoleLockRemaining == 0) {
			consoleLocked = 0;
		}
	}
}

float readTemperature(void) {
	uint8_t reg = 0x00;
	uint8_t data = 0;
	
	I2C_SendData(I2C1, TC74_ADDRESS << 1, &reg, 1);
	I2C_ReceiveData(I2C1, TC74_ADDRESS << 1, &data, 1);
	
	return (float)(int8_t)data;
}

static int isDoorClosed(double y) {
	return ((y > ACC_THRESHOLD || y < -ACC_THRESHOLD));
}

static int isDoorOpen(double z) {
	return ((z > ACC_THRESHOLD || z < -ACC_THRESHOLD));
}

static void moveDoorClosed(void) {
    sprintf(buffer, "Door closing.\r\n");
    UART_print(buffer);

    uint32_t start = getTick();
    uint32_t lastStep = getTick();
    uint8_t stableCount = 0; 
    setDire(-1);

    while(1) {
        uint32_t now = getTick();
        if (now - lastStep >= 20) {
            lastStep = now;
            double x, y, z;
            readValues(&x, &y, &z);
            if (isDoorClosed(y)) {
                stableCount++;
                if (stableCount >= 1) {
                    setDire(0);
                    doorState = DOOR_CLOSED;
                    sprintf(buffer, "Door closed.\r\n");
                    UART_print(buffer);
                    return;
                }
            } else {
                stableCount = 0;
            }
        }
        if (getTick() - start > DOOR_MOVE_TIMEOUT_MS) {
            setDire(0);
            sprintf(buffer, "Door close timeout.\r\n");
            UART_print(buffer);
            return;
        }
    }
}

static void moveDoorOpen(void) {
    sprintf(buffer, "Door opening.\r\n");
    UART_print(buffer);

    uint32_t start = getTick();
    uint32_t lastStep = getTick();
    uint8_t stableCount = 0;
    setDire(1);

    while(1) {
        uint32_t now = getTick();
        if (now - lastStep >= 20) {
            lastStep = now;
            double x, y, z;
            readValues(&x, &y, &z);
            if (isDoorOpen(z)) {
                stableCount++;
                if (stableCount >= 1) {
                    setDire(0);
                    doorState = DOOR_OPEN;
                    sprintf(buffer, "Door opened.\r\n");
                    UART_print(buffer);
                    return;
                }
            } else {
                stableCount = 0;
            }
        }
        if (getTick() - start > DOOR_MOVE_TIMEOUT_MS) {
            setDire(0);
            sprintf(buffer, "Door open timeout.\r\n");
            UART_print(buffer);
            return;
        }
    }
}

void UART_onInput(char* inputs, uint32_t size) {	
		if (size == 0) return;

    if (strncmp(inputs, "o\n", 2) == 0 || strncmp(inputs, "o\r\n", 3) == 0) {
        cmdOpen = 1;
    } else if (strncmp(inputs, "c\n", 2) == 0 || strncmp(inputs, "c\r\n", 3) == 0) {
        cmdClose = 1;
    } else if (strncmp(inputs, "s\n", 2) == 0 || strncmp(inputs, "s\r\n", 3) == 0) {
        cmdStop = 1;
    } else {
        static char inputBuffer[IO_SIZE];
        sprintf(inputBuffer, "Unknown: %s\r\n", inputs);
        UART_print(inputBuffer);
    }
}

int main(void) {
    System_Clock_Init(); 
    Motor_Init();
    SysTick_Init();
    
    UART2_GPIO_Init();
    UART2_Init();      
    USART_Init(USART2);
    USART2->CR3 |= USART_CR3_DMAT;
    USART2->CR1 |= USART_CR1_TCIE;
    USART2->CR1 |= USART_CR1_RXNEIE;
    while (!(USART2->ISR & USART_ISR_TEACK));
    while (!(USART2->ISR & USART_ISR_REACK));
    NVIC_SetPriority(USART2_IRQn, 3);
    NVIC_SetPriority(DMA1_Channel7_IRQn, 2);
    NVIC_EnableIRQ(USART2_IRQn);
    
		UART1_GPIO_Init();
    UART1_Init();      
    USART_Init(USART1);
		USART1->CR3 |= USART_CR3_DMAT;
    USART1->CR1 |= USART_CR1_TCIE;
    USART1->CR1 |= USART_CR1_RXNEIE;
		while (!(USART1->ISR & USART_ISR_TEACK));
    while (!(USART1->ISR & USART_ISR_REACK));
    NVIC_SetPriority(USART1_IRQn, 3);
    NVIC_SetPriority(DMA1_Channel4_IRQn, 2);
    NVIC_EnableIRQ(USART1_IRQn);
	
    LED_Init();
    SPI1_GPIO_Init();
    SPI1_Init();
    initAcc();
    I2C_GPIO_Init();
    I2C_Initialization();
    
    sprintf(buffer, "Program Starts.\r\nCommands: 'o'=open, 'c'=close, 's'=stop\r\n");
    UART_print(buffer);
    
    moveDoorClosed();
    
    float lastTemp = -999.0f;
    uint32_t lastMeasure = getTick();
		
		uint8_t tempHigh = 0;
    
    while(1) {
        if (cmdOpen) {
            cmdOpen = 0;
            consoleLocked = 1;
            consoleLockRemaining = CONSOLE_LOCK_MS;
            sprintf(buffer, "Opening (CW)\r\n");
            UART_print(buffer);
            moveDoorOpen();
        } else if (cmdClose) {
            cmdClose = 0;
            consoleLocked = 1;
            consoleLockRemaining = CONSOLE_LOCK_MS;
            sprintf(buffer, "Closing (CCW)\r\n");
            UART_print(buffer);
            moveDoorClosed();
        } else if (cmdStop) {
            cmdStop = 0;
            consoleLocked = 1;
            consoleLockRemaining = CONSOLE_LOCK_MS;
            setDire(0);
            sprintf(buffer, "Stopped\r\n");
            UART_print(buffer);
        }

        uint32_t now = getTick();
        if (now - lastMeasure >= MEASURE_INTERVAL_MS) {
            lastMeasure = now;

            double x, y, z;
            readValues(&x, &y, &z);
            float temp = readTemperature();
					
						//sprintf(buffer, "X: %.2f Y: %.2f Z: %.2f\r\n", x, y, z);
						//UART_print(buffer);

            if ((int)(temp * 2) != (int)(lastTemp * 2)) {
                sprintf(buffer, "Temp: %.1f C\r\n", temp);
                UART_print(buffer);
                lastTemp = temp;
            }

            if (!consoleLocked) {
							if (!tempHigh && temp > TEMP_OPEN_THRESHOLD) { 
									tempHigh = 1;
									sprintf(buffer, "Temperature too high. Door opening.\r\n");
									UART_print(buffer);
									moveDoorOpen();
							} else if (tempHigh && temp < TEMP_CLOSE_THRESHOLD) { 
								tempHigh = 0;
								sprintf(buffer, "Temperature too low. Door closing.\r\n");
								UART_print(buffer);
								moveDoorClosed();
							}
						}				
            LED_Toggle();
        }
			}
}