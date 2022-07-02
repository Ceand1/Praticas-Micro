
/**
 * @file    Switch.c
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

typedef struct {
	uint32_t PCR[32];
}PORTRegs_t;

#define PORT_A ((PORTRegs_t *)0x40049000)
#define PORT_B ((PORTRegs_t *)0x4004A000)
#define PORT_C ((PORTRegs_t *)0x4004B000)
#define PORT_D ((PORTRegs_t *)0x4004C000)
#define PORT_E ((PORTRegs_t *)0x4004D000)

typedef struct {
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


typedef struct {
	uint32_t iser[1];
	uint32_t rsvd[31];
	uint32_t icer[1];
	uint32_t rsvd1[31];
	uint32_t ispr[1];
	uint32_t rsvd2[31];
	uint32_t icpr[1];
	uint32_t rsvd3[31];
	uint32_t rsvd4[64];
	uint32_t ipr[8];
}NVIC_Regs_t;

#define NVIC_REG ((NVIC_Regs_t *)0xE000E100)

void PORTC_PORTD_IRQHandler(void)
{
	if(GPIO_D->PDIR & (1 << 4)) {
		// Fazer o LED -> OFF
		GPIO_E->PSOR = (1 << 0);

	}else{
		GPIO_E->PCOR = (1 << 0);
	}
	// W1C em Interrupt Status Flag da PORTA_D_4
	PORT_D->PCR[4] = PORT_D->PCR[4] | (1 << 24);
}

void PORTA_IRQHandler(void)
{
	if(GPIO_A->PDIR & (1 << 2)) {
		// Fazer o LED -> OFF
		GPIO_A->PSOR = (1 << 1);

	}else{
		GPIO_A->PCOR = (1 << 1);
	}


	// W1C em Interrupt Status Flag da PORTA_A_2
	PORT_A->PCR[2] = PORT_A->PCR[2] | (1 << 24);

}

/*
 * @brief   Application entry point.
 */
int main(void) {
	// Ativando o clock da PORTA_A, PORTA_B, PORTA_C, PORTA_D e PORTA_E
	SIM->SCGC5 = (1 << 9) | (1 << 11) | (1 << 12) | (1 << 13);

	// Configurando o LED PORTA_A_1 como GPIO
	PORT_A->PCR[1] = (1 << 8);

	// Configurando Switch PORTDA_A_2 para SW GPIO / Pull Enable / Pull UP
	PORT_A->PCR[2] = (1 << 8) | (1 << 1) | (1 << 0) | (0xb << 16);

	// Configurando LED PORTA_A_1 como saída
	GPIO_A->PDDR = (1 << 1);

	// Fazer o LED -> OFF
	GPIO_A->PSOR = (1 << 1);

	// Configurando o LED PORTA_E_0 como GPIO
	PORT_E->PCR[0] = (1 << 8);

	// Configurando Switch PORTDA_D_4 para SW GPIO / Pull Enable / Pull UP
	PORT_D->PCR[4] = (1 << 8) | (1 << 1) | (1 << 0) | (0xb << 16);

	// Configurando LED PORTA_E_0 como saída
	GPIO_E->PDDR = (1 << 0);

	// Fazer o LED -> OFF
	GPIO_E->PSOR = (1 << 0);

	// Ativar a Interrupção para PORTA_D e PORTA_A
	NVIC_REG->iser[0] |= (1 << 31) | (1 << 30);

	while(1) {
	}
    return 0 ;
}
