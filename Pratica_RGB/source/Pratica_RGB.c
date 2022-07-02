/**
 * @file    Switch_LED.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */
typedef struct{
	uint32_t PCR[32];
}PORTRegs_t;

#define PORT_A ((PORTRegs_t *)0x40049000)
#define PORT_B ((PORTRegs_t *)0x4004A000)
#define PORT_C ((PORTRegs_t *)0x4004B000)
#define PORT_D ((PORTRegs_t *)0x4004C000)
#define PORT_E ((PORTRegs_t *)0x4004D000)

typedef struct{
	 uint32_t PDOR;
	 uint32_t PSOR;
	 uint32_t PCOR;
	 uint32_t PTOR;
	 uint32_t PDIR;
	 uint32_t PDDR;
}GPIORegs_t;

#define GPIO_A ((GPIORegs_t *)0x400FF000)
#define GPIO_B ((GPIORegs_t *)0x400FF040)
#define GPIO_C ((GPIORegs_t *)0x400FF080)
#define GPIO_D ((GPIORegs_t *)0x400FF0C0)
#define GPIO_E ((GPIORegs_t *)0x400FF100)

void delay(int n){
	while(n>0){
		n--;
	}
}
/*
 * @brief   Application entry point.
 */
int main(void) {
		SIM->SCGC5 = (1 << 10) | (1 << 12);

		//configura os pinos como GPIO
		PORT_B->PCR[18] = (1 << 8);
		PORT_B->PCR[19] = (1 << 8);
		PORT_D->PCR[1] = (1 << 8);

		//configura os pinos como saída
		GPIO_B->PDDR |= (1 << 18);
		GPIO_B->PDDR |= (1 << 19);
		GPIO_D->PDDR |= (1 << 1);

		//liga e desliga vermelho e verde
		GPIO_B->PSOR |= (1 << 18);
		GPIO_B->PSOR |= (1 << 19);

		delay(1000000);

		GPIO_B->PCOR |= (1 << 18);
		GPIO_B->PCOR |= (1 << 19);

		delay(1000000);

		//liga e desliga vermelho e azul
		GPIO_B->PSOR |= (1 << 18);
		GPIO_D->PSOR |= (1 << 1);

		delay(1000000);

		GPIO_B->PCOR |= (1 << 18);
		GPIO_D->PCOR |= (1 << 1);

		delay(1000000);

		//liga verde e azul
		GPIO_B->PSOR |= (1 << 19);
		GPIO_D->PSOR |= (1 << 1);

		delay(1000000);

		GPIO_B->PCOR |= (1 << 19);
		GPIO_D->PCOR |= (1 << 1);

		delay(1000000);

		//liga as 3 cores
		GPIO_B->PSOR |= (1 << 18);
		GPIO_B->PSOR |= (1 << 19);
		GPIO_D->PSOR |= (1 << 1);

		delay(1000000);

		GPIO_B->PCOR |= (1 << 18);
		GPIO_B->PCOR |= (1 << 19);
		GPIO_D->PCOR |= (1 << 1);


    return 0 ;
}

