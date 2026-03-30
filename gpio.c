/*
 * gpio.c
 *
 *  Created on: 30 mars 2026
 *      Author: mi11p002
 */

#include "gpio.h"
#include <stdint.h>

void gpio_init(char port) {
	if(port != 'I' && port != 'J')
		return;

	volatile uint32_t* baseRCC = (volatile uint32_t *)0x58024400; //PAGE 138
	volatile uint32_t* RCC_AHB4ENR = baseRCC + 0x0E0/4; //ADRESS OF RCC_AHB4ENR
	// TO FIND RCC_GPIOxENR WE ARE GOING TO FIND THE 9 AND 10th BIT

	if(port == 'I') //Check page 492
	{
		*RCC_AHB4ENR = *RCC_AHB4ENR | 0x00000100; //BECAUSE RCC_GPIOIENR IS THE 9TH BIT OF RCC_AHB4ENR
		while((*RCC_AHB4ENR & 0x00000100) != 0x00000100);
	}

	if(port == 'J')
	{
		*RCC_AHB4ENR = *RCC_AHB4ENR | 0x00000200; //BECAUSE RCC_GPIOJENR IS THE 9TH BIT OF RCC_AHB4ENR
		while((*RCC_AHB4ENR & 0x00000200) != 0x00000200);
	}
	//!= HAVE A HIGHER PRIORITY THAN &

}

void gpio_mode(char port, int bit, int dir) {
	if((port != 'I' && port != 'J') || (bit < 0 || bit > 15) || (dir != 0 && dir != 1))
			return;

	volatile uint32_t* GPIO_BASE;
	volatile uint32_t mask = 0b1; //mask = 0000 0000 0000 0000 0000 0000 0000 0001

	if(port == 'I') GPIO_BASE = (volatile uint32_t*)0x58022000; //PAGE 138
	if(port == 'J') GPIO_BASE = (volatile uint32_t*)0x58022400; //PAGE 138

	if(dir == 1) // SET THE BIT PAIR TO 00
	{
		*GPIO_BASE&= ~(mask << (2*bit)); // PUT THE 2*BIT BIT TO 0
		// 0000 0000 0000 0000 0000 0010 0000 0000
		// 1111 1111 1111 1111 1111 1101 1111 1111
		*GPIO_BASE&= ~(mask << (2*bit + 1)); // PUT THE 2*BIT+1 BIT TO 0
	}
	else // SET THE BIT PAIR TO 01
	{
		*GPIO_BASE|= mask << (2*bit); // SET THE 2*BIT BIT TO 1
		// 0000 0000 0000 0000 0000 0010 0000 0000
		// 1111 1111 1111 1111 1111 1101 1111 1111
		*GPIO_BASE&= ~(mask << (2*bit+1)); // SET THE 2*BIT+1 BIT TO 0
	}
	// GPIO BASE IS ALSO THE BEGINNIG OF GPIOxMODER REGISTER SO WE DON'T NEED TO MAKE A OFFSET
	// != HAVE A HIGHER PRIORITY THAN &
}


void gpio_set(char port, int n, int value) //WE ARE GOING TO USE THE REG ODR
	{
	if((port != 'I' && port != 'J') || (n < 0 || n > 15) || (value != 0 && value != 1))
				return;

	// gpio_mode(port,n,1); //CONFIG THE BIT TO OUTPUT MODE IF NOT YET IN OUTPUT

	volatile uint32_t* GPIO_BASE;
	volatile uint32_t mask = 0b1; //mask = 0000 0000 0000 0000 0000 0000 0000 0001
	volatile uint32_t* GPIO_ODR;

	if(port == 'I') GPIO_BASE = (volatile uint32_t*)0x58022000; //PAGE 138
	if(port == 'J') GPIO_BASE = (volatile uint32_t*)0x58022400; //PAGE 138

	GPIO_ODR = GPIO_BASE + 0x14/4; //0x14 THE OFFSET

	if(value == 0) // SET THE TO 0
	{
		*GPIO_ODR&= ~(mask << n); // SET THE 2*BIT n TO 0
		// 0000 0000 0000 0000 0000 0010 0000 0000
		// 1111 1111 1111 1111 1111 1101 1111 1111
	}
	else // SET THE BIT n TO 1
	{
		*GPIO_ODR|= mask << n;
		// 0000 0000 0000 0000 0000 0010 0000 0000
		// 1111 1111 1111 1111 1111 1101 1111 1111
	}
}

int gpio_get(char port, int bit) {  //WE ARE GOING TO USE THE REG IDR
	if((port != 'I' && port != 'J') || (bit < 0 || bit > 15))
					return -1;

	volatile uint32_t* GPIO_BASE;
	volatile uint32_t* GPIO_IDR;
	volatile uint32_t mask = 0b1; //mask = 0000 0000 0000 0000 0000 0000 0000 0001

	if(port == 'I') GPIO_BASE = (volatile uint32_t*)0x58022000; //PAGE 138
	if(port == 'J') GPIO_BASE = (volatile uint32_t*)0x58022400; //PAGE 138

	GPIO_IDR = GPIO_BASE + 0x10/4; //0x10 THE OFFSET

	if((*GPIO_IDR & (mask << bit)) == (mask << bit))
		return 1;

	return 0;
}

void gpio_toggle(char port, int bit) {
    if((port != 'I' && port != 'J') || (bit < 0 || bit > 15))
        return;

    volatile uint32_t* GPIO_BASE;
    volatile uint32_t* GPIO_ODR;
    volatile uint32_t mask = 0b1; //mask = 0000 0000 0000 0000 0000 0000 0000 0001

    if(port == 'I') GPIO_BASE = (volatile uint32_t*)0x58022000; //PAGE 138
    if(port == 'J') GPIO_BASE = (volatile uint32_t*)0x58022400; //PAGE 138

    GPIO_ODR = GPIO_BASE + 0x14/4;

    *GPIO_ODR ^= (mask << bit); // XOR WITH 0 KEEP THE VALUE AND CHANGE IT WITH ONE
}
