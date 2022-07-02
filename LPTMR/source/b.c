///**
// * @file    LPTMR.c
// * @brief   Application entry point.
// */
//#include <stdio.h>
//#include "board.h"
//#include "peripherals.h"
//#include "pin_mux.h"
//#include "clock_config.h"
//#include "MKL43Z4.h"
//#include "fsl_debug_console.h"
///* TODO: insert other include files here. */
//
///* TODO: insert other definitions and declarations here. */
//
//typedef struct{
//	uint32_t PCR[32];
//}PORTRegs_t;
//
//#define PORT_B ((PORTRegs_t *)0x4004A000)
//#define PORT_D ((PORTRegs_t *)0x4004C000)
//
//typedef struct{
//	uint32_t PDOR;
//	uint32_t PSOR;
//	uint32_t PCOR;
//	uint32_t PTOR;
//	uint32_t PDIR;
//	uint32_t PDDR;
//}GPIORegs_t;
//
//#define GPIO_B ((GPIORegs_t *)0x400FF040)
//#define GPIO_D ((GPIORegs_t *)0x400FF0C0)
//
//typedef struct {
//	uint32_t iser[1];
//	uint32_t rsvd[31];
//	uint32_t icer[1];
//	uint32_t rsvd1[31];
//	uint32_t ispr[1];
//	uint32_t rsvd2[31];
//	uint32_t icpr[1];
//	uint32_t rsvd3[31];
//	uint32_t rsvd4[64];
//	uint32_t ipr[8];
//}NVIC_Regs_t;
//
//#define NVIC_REG ((NVIC_Regs_t *)0xE000E100)
//
//typedef struct{
//	uint32_t tcsr;
//	uint32_t prescale;
//	uint32_t compare;
//	uint32_t count;
//}LPTMR_Regs_t;
//
//#define LPTMR_REG ((LPTMR_Regs_t *) 0x40040000)
//
//void LPTMR0_IRQHandler(void){
//	//Alternar pino 18 do led red para acender e apagar
//	GPIO_D->PTOR = (1 << 5);
//
//	// Timer Compare Flag
//	//0 The value of CNR is not equal to CMR and increments --
//	//1 The value of CNR is equal to CMR and increments.
//	/*
//	 * 	Low Power Timer Control Status Register (LPTMRx_CSR)
//	 *| 31  --  8 |  7  |  6  | 5  4 |  3  |  2  |  1  |  0  |
//	 *|     0	  | ISF | TIE |  TPS | TPP | TFC | TMS | TEN |
//	 *|           | w1c |
//	 */
//	LPTMR_REG->tcsr = LPTMR_REG->tcsr | (1 << 7);
//}
//
///*
// * @brief   Application entry point.
// */
//
////1A - Acender led sem preescale (1s)
//int main(void) {
//	//1 - Ativar o clock para LPTMR e para porta B
//	/*
//	 * 	System Clock Gating Control Register 5 (SIM_SCGC5)
//	 *| 31  --  20| 19 | 18 -- 14 | 13 | 12 | 11 | 10 |  9 | 8  7 | 6 |   5 | 4  3  2 | 1 |   0   |
//	 *|     0	  |  0 |     0    | PE | PD | PC | PB | PA |   1  | 0 | TSI	|    0    | 0 | LPTMR |
//	 */
//	SIM->SCGC5 = (1 << 0) | (1 << 10) | (1 << 12);
//
//	//2 - Port_B_18 como GPIO
//	/*
//	 * 	Pin Control Register n (PORTx_PCRn)
//	 *| 31  --  25|  24 | 23 -- 20 | 19 -- 16 | 15 -- 11 | 10  9  8 | 7 |  6  | 5 |  4  | 3 |  2  | 1  |  0 |
//	 *|     0	  | ISF |     0    |   IRQC   |     0    |    MUX   | 0 | DSE | 0 | PFE | 0 | SRE | PE | PS |      | 0 | LPTMR |
//	 *|           | w1c |		   |
//	 */
//	PORT_D->PCR[5] = (1 << 8);
//
//	//GPIO_B_18 como saída
//	GPIO_D->PDDR = (1 << 5);
//
//
//	//Bypass é ativado e clock do LPTMR é LPO
//	/*
//	 * 	Low Power Timer Prescale Register (LPTMRx_PSR)
//	 *| 31  --  7 |  6  5  4  3  |   2  | 1   0 |
//	 *|     0	  |   PRESCALE   | PBYP |  PCS  |
//	 */
//	LPTMR_REG->prescale &= ~(1 << 2);
//	//LPTMR_REG->prescale |= (1 << 0);
//	LPTMR_REG->prescale = (1 << 0) | (0b0101 << 3);
//
//	//LPO é 1kHz -> T = 1/f = 1/1000 = 1ms. Queremos o tempo de 1s -> compare register = 1/1ms = 1000s
//	LPTMR_REG->compare = 124;
//
//
//	//Ativar Timer Enable -- 0 LPTMR is disabled and internal logic is reset -- 1 LPTMR is enabled
//	//Timer Interrupt Enable -- 0 Timer interrupt disabled -- 1 Timer interrupt enabled
//	/*
//	 * 	Low Power Timer Control Status Register (LPTMRx_CSR)
//	 *| 31  --  8 |  7  |  6  | 5  4 |  3  |  2  |  1  |  0  |
//	 *|     0	  | ISF | TIE |  TPS | TPP | TFC | TMS | TEN |
//	 *|           | w1c |
//	 */
//	LPTMR_REG->tcsr = (1 << 0) | (1 << 6);
//
//	//NVIC_EnableIRQ(LPTMR0_IRQn);
//	NVIC_REG->iser[0] = (1 << 28);
//
//	while(1){
//	}
//
//    return 0 ;
//}
//
