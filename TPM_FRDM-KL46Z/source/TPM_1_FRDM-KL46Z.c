///**
// * @file    TPM.c
// * @brief   Application entry point.
// */
//#include <stdio.h>
//#include "board.h"
//#include "peripherals.h"
//#include "pin_mux.h"
//#include "clock_config.h"
//#include "MKL46Z4.h"
//#include "fsl_debug_console.h"
//
///* TODO: insert other include files here. */
//
///* TODO: insert other definitions and declarations here. */
//
//typedef struct{
//	uint32_t PCR[32];
//}PORTRegs_t;
//
//#define PORT_B ((PORTRegs_t *)0x4004A000)
//#define PORT_E ((PORTRegs_t*) 0x4004D000)
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
//#define GPIO_B ((GPIORegs_t*) 0x400FF040)
//#define GPIO_E ((GPIORegs_t*) 0x400FF100)
//
//typedef struct {
//	uint32_t ISER[1];
//	uint32_t RSVD[31];
//	uint32_t ICER[1];
//	uint32_t RSVD1[31];
//	uint32_t ISPR[1];
//	uint32_t RSVD2[31];
//	uint32_t ICPR[1];
//	uint32_t RSVD3[31];
//	uint32_t RSVD4[64];
//	uint32_t IPR[1];
//}NVIC_Regs_t;
//
//#define NVIC_REG ((NVIC_Regs_t *)0xE000E100)
//
//typedef struct{
//	uint32_t SC;
//	uint32_t CNT;
//	uint32_t MOD;
//}TPMR_Regs_t;
//
//#define TPM_REG ((TPMR_Regs_t *) 0x40038000)
//
//typedef struct{
//	uint32_t STATUS;
//	uint32_t CONF;
//}TPM_CONF_Regs_t;
//
//#define TPM_CONF_REG ((TPM_CONF_Regs_t *) 0x40038050)
//
//typedef struct{
//	uint32_t SOPT2;
//}SOPT2R_Regs_t;
//
//#define SOPT2_REG ((SOPT2R_Regs_t *) 0x40048004)
//
//void TPM0_IRQHandler(void){
//	if((TPM_REG->SC & (1 << 7))){
//		GPIO_E->PTOR = (1 << 29);
//
//		//(TOF) Timer Overflow Flag -- 1 : LPTPM counter has overflowed.
//		TPM_REG->SC |= (1 << 7);
//
//		//TPM_REG->CNT = 0x0000;
//		TPM_REG->CNT = 0x0000; // resetar
//	}
//}
//
///*
// * @brief   Application entry point.
// */
//int main(void) {
//
//	// Habilitando o clock de 32kHz
//	MCG->C1 |= (1 << 1);
//
//	/*****************************************************************************/
//	//pagina 195
//	//(TPMSRC) clock source select
//	//Selects the clock source for the TPM counter clock
//	// 01  MCGFLLCLK clock or MCGPLLCLK/2
//	SOPT2_REG->SOPT2 |= (0b11 << 24);//clock MCGFLLCLK ?? de 20,97
//	//PLL/FLL clock select
//	//Selects the MCGPLLCLK or MCGFLLCLK clock for various peripheral clocking options.
//	// 0  MCGFLLCLK clock
//	SIM->SCGC6 |= (1 << 24);
//
//	/*
//	 * Configura????o TPM
//	 *
//	 */
//	//(TOIE) Timer Overflow Interrupt Enable -- Enables LPTPM overflow interrupts.
//	// 0  Disable TOF interrupts. Use software polling or DMA request.
//	// 1  Enable TOF interrupts. An interrupt is generated when TOF equals one.
//	TPM_REG->SC |= (1 << 6);//ativando a interrup????o para TOF
//
//	TPM_REG->CNT = 0x0000; // resetar
//
//	//TPM_REG->MOD = TPM_CLOCK / (1 << (TPM_clk_PRESCALE + 1))/TPM_OVerflow_frequency
//	// MOD = (32768Hz/32)*1 = 1024
//
//
//	//(PS)  Prescaler
//	TPM_REG->SC |= (0b000 << 0); // desativando o preescale
//	TPM_REG->MOD = 32000; //coloca a frequencia do pino, 32kHz, (MCG->C1)
//
//	//(CMOD) Clock Mode Selection -- 01 : LPTPM counter increments on every LPTPM counter clock
//	TPM_REG->SC |= (0b01 << 3);//incrementa a cada pulso do LPTPM
//
//	/*****************************************************************************/
//
//	SIM->SCGC5 |= (1 << 13); //ativar clock porta E
//
//	/*****************************************************************************/
//
//	//Port_E_31 como GPIO
//	PORT_E->PCR[29] |= (1 << 8);
//
//	//GPIO_E_31 como sa??da
//	GPIO_E->PDDR |= (1 << 29);
//
//	/*****************************************************************************/
//
//	//NVIC_EnableIRQ(TPM0_IRQn);
//	NVIC_REG->ISER[0] = (1 << 17);
//
//	while(1){
//	}
//
//
//	return 0 ;
//}
//
