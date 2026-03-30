/*
 * timer.c
 *
 *  Created on: 30 mars 2026
 *      Author: mi11p002
 */

#include <stdint.h>

void timer_init(int n)
{
	if(n < 2 || (n > 7 && n < 12) || n > 14)
		return;

	int bit = (n > 7 ? n-6 : n-2);

	volatile uint32_t* baseRCC = (volatile uint32_t *)0x58024400; 	// PAGE 138
	volatile uint32_t* RCC_APB1LENR = baseRCC + 0x0E8/4;			// ADRESS OF RCC_APB1LENR

	volatile uint32_t mask = (1 << bit);

	*RCC_APB1LENR = *RCC_APB1LENR | mask;		// BECAUSE WE WANT TO SET THE (bit)TH BIT OF RCC_APB1LENR
	while((*RCC_APB1LENR & mask) != mask);		// != HAVE A HIGHER PRIORITY THAN &
}

void timer_config(int n, int d) {
	(void)n; // IGNORE FOR NOW
	(void)d;

	// p1826
	volatile uint32_t* TIM2_CR1 = (volatile uint32_t *)0X40000000;	// ADRESS OF TIM2_CR1 ref. p141
	*TIM2_CR1 = *TIM2_CR1 | 1; 										// SET BIT CEN TO 1
	while((*TIM2_CR1 & 1) != 1);

	volatile uint32_t* TIM2_PSC = TIM2_CR1 + 0x28 / 4; 				// ADRESS OF TIM2_PSC ref. p1844
	*TIM2_PSC = 63u;												// SET PSC TO 63(base 10) -> 64MHZ/(63+1)=1MHZ
	while(*TIM2_PSC != 63u);

	volatile uint32_t* TIM2_ARR = TIM2_CR1 + 0x2C / 4; 				// ADRESS OF TIM2_ARR ref. p1845
	*TIM2_ARR = 1000000u;
	while(*TIM2_ARR != 1000000u);
}

void timer_wait(int n) {
	(void)n;
	volatile uint32_t* TIM2_SR = (volatile uint32_t *)0X40000010;
	while(!(*TIM2_SR & 1));
	*TIM2_SR = *TIM2_SR & ~1u;
}
