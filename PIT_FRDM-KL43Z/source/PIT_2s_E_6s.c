
/**
 * @file    PIT_FRDM-KL43Z.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL43Z4.h"
#include "fsl_debug_console.h"



/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */
typedef struct {
	uint32_t MCR;
	uint32_t LTM64H;
	uint32_t LTM64L;
}PIT_Regs_t;

#define PIT_REG ((PIT_Regs_t*) 0x40037000)

typedef struct {
	uint32_t LDVAL;
	uint32_t CVAL;
	uint32_t TCTRL;
	uint32_t TFLAG;
}PIT_Chnl_Regs_t;

#define PIT_CH_0_REG ((PIT_Chnl_Regs_t*) 0x40037100)
#define PIT_CH_1_REG ((PIT_Chnl_Regs_t*) 0x40037110)

typedef struct {
	uint32_t PCR[32];
}PORTRegs_t;

#define PORT_B ((PORTRegs_t*) 0x4004A000)
#define PORT_D ((PORTRegs_t*) 0x4004C000)
#define PORT_E ((PORTRegs_t*) 0x4004D000)

typedef struct {
	uint32_t PDOR;
	uint32_t PSOR;
	uint32_t PCOR;
	uint32_t PTOR;
	uint32_t PDIR;
	uint32_t PDDR;
}GPIORegs_t;

#define GPIO_B ((GPIORegs_t*) 0x400FF040)
#define GPIO_D ((GPIORegs_t*) 0x400FF0C0)
#define GPIO_E ((GPIORegs_t*) 0x400FF100)


typedef struct {
	uint32_t ISER[1];
	uint32_t RSVD[31];
	uint32_t ICER[1];
	uint32_t RSVD1[31];
	uint32_t ISPR[1];
	uint32_t RSVD2[31];
	uint32_t ICPR[1];
	uint32_t RSVD3[31];
	uint32_t RSVD4[64];
	uint32_t IPR[1];
}NVIC_Regs_t;

#define NVIC_REG ((NVIC_Regs_t*) 0xE000E100)

#define GET_SEC_COUNT(x) (CLOCK_GetBusClkFreq() * x)

void PIT_IRQHandler(void) {
	//Checa e a interrupção no canal foi gerada
	if(PIT_CH_0_REG->TFLAG & (1 << 0)) {
		GPIO_E->PTOR |= (1 << 31);
		PIT_CH_0_REG->TFLAG |= (1 << 0);
	} else {
		GPIO_D->PTOR |= (1 << 5);
		PIT_CH_1_REG->TFLAG |= (1 << 0);
	}
}
/*
 * @brief   Application entry point.
 */
int main(void) {
	uint32_t clock = CLOCK_GetBusClkFreq();

	SIM->SCGC5 |= (1 << 13) | (1 << 12);

	PORT_E->PCR[31] |= (1 << 8);
	GPIO_E->PDDR |= (1 << 31);
	GPIO_E->PSOR |= (1 << 31);

	PORT_D->PCR[5] |= (1 << 8);
	GPIO_D->PDDR |= (1 << 5);
	GPIO_D->PSOR |= (1 << 5);

	SIM->SCGC6 |= (1 << 23);
	PIT_REG->MCR = 0;
	PIT_CH_0_REG->LDVAL = GET_SEC_COUNT(2);
	PIT_CH_1_REG->LDVAL = GET_SEC_COUNT(6);


	PIT_CH_0_REG->TCTRL = (1 << 1) | (1 << 0);
	PIT_CH_1_REG->TCTRL = (1 << 1) | (1 << 0);

	NVIC_REG->ISER[0] |= (1 << 22);
	printf("Clock: %d\n", clock);
	while(1){
	}

    return 0 ;
}
