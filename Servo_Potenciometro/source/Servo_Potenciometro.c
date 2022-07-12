#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"


#define MIN_VALUE 60
#define MAX_VALUE 330
#define PTB0_PIN 0


void InitTPMPWM(void){
	/*
	 * 	System Clock Gating Control Register 5 (SIM_SCGC5)
	 *| 31  --  20| 19 | 18 -- 14 | 13 | 12 | 11 | 10 |  9 | 8  7 | 6 |   5 | 4  3  2 | 1 |   0   |
	 *|     0	  |  0 |     0    | PE | PD | PC | PB | PA |   1  | 0 | TSI	|    0    | 0 | LPTMR |
	 */
	SIM->SCGC5 = (1 << 10); //ativar clock porta B
	/*
	 * 	Pin Control Register n (PORTx_PCRn)
	 *| 31  --  25|  24 | 23 -- 20 | 19 -- 16 | 15 -- 11 | 10  9  8 | 7 |  6  | 5 |  4  | 3 |  2  | 1  |  0 |
	 *|     0	  | ISF |     0    |   IRQC   |     0    |    MUX   | 0 | DSE | 0 | PFE | 0 | SRE | PE | PS |      | 0 | LPTMR |
	 *|           | w1c |		   |
	 */
	PORTB->PCR[PTB0_PIN] = 	(1 << 24) |		// ISF=PORTB_PCR18[24]: w1c (limpa a pendência)
							(0b011 << 8);   // MUX=PORTB_PCR18[10:8]=0b011 (TPM2_CH0)
   // MUX=PORTB_PCR18[10:8]=0b011 (TPM2_CH0)
	/*
	 * 	System Clock Gating Control Register 6 (SIM_SCGC6)
	 *|   31 | 30 |  29 | 28 |  27  |  26  |  25  |  24  |  23 | 22 -- 2 |    1   |  0  |
	 *| DAC0 |  0 | RTC |  0 | ADC0 | TPM2 | TPM1 | TPM0 | PIT |    0    | DMAMUX | FTF |
	 */
	SIM->SCGC6 = (1 << 25);

	/*
	 * 	System Options Register 2 (SIM_SOPT2)
	 *|   31 -- 28 |   27  26 | 25  24 | 23 -- 19 |   18   | 17 |     16    | 15 -- 8 |   7 -- 5  |     4      | 3 -- 0 |
	 *|      0     | UART0SRC | TPMSRC |    0     | USBSRC |  0 | PLLFLLSEL |     0   | CLKOUTSEL | RTCCLKOUTS |    0   |
	 */
	//PLL/FLL clock select
	SIM->SOPT2 &= ~(1 << 16);	//0 MCGFLLCLK clock
									//1 MCGPLLCLK clock with fixed divide by two
	/*TPMSRC	 *
	 * TPM clock source select
	 * 	Selects the clock source for the TPM counter clock
	 * 	00 Clock disabled
	 * 	01 MCGFLLCLK clock or MCGPLLCLK/2
	 * 	10 OSCERCLK clock
	 * 	11 MCGIRCLK clock
	*/
	SIM->SOPT2 |= (0b01 << 24);	//(TPMSRC) clock source select
									//Selects the clock source for the TPM counter clock
									// 01  MCGFLLCLK clock or MCGPLLCLK/2

	//valor do modulo = 20971520 / 128 = 163840 / 3276 = 50 Hz
	TPM1->MOD |= 3276;     		// MOD=TPM2_MOD[15:0]=3276

	//valor do modulo = 48000000 / 128 = 375000 / 7500 = 50 Hz
	//TPM1_REG->mod = 7500;    		// MOD=TPM2_MOD[15:0]=7500

	/*
	 * 	Status and Control (TPMx_SC)
	 *|   31 -- 9 |  8  |  7  |   6  |   5   | 4  3 | 2  1  0 |
	 *|      0    | DMA | TOF | TOIE | CPWMS | CMOD |   PS    |
	 *|			  |     | w1c |
	 */
	TPM1->SC	= 	(0 << 5)	|			// CPWMS=TPM2_SC[5]=0 (modo de contagem crescente)
					(0b01 << 3)	|           // CMOD=TPM2_SC[4:3]=0b01 (incrementa a cada pulso do LPTPM)
					(0b111 << 0);           		// PS=TPM2_SC[2:0]=0b101 (Fator Prescale = 32)

	/* Desativar o PWM no modulo TPM1 canal 0 --> PTB0 */
	TPM1->CONTROLS[0].CnSC	   = (0 << 5) | // MSB =TPM2_C0SC[5]=0
								 (0 << 4) | // MSA =TPM2_C0SC[4]=0
								 (0 << 3) | // ELSB=TPM2_C0SC[3]=0
								 (0 << 2);  // ELSA=TPM2_C0SC[2]=0

	/* Ativar o PWM no modulo TPM1 canal 0 --> PTB0 */
	TPM1->CONTROLS[0].CnSC     = (1 << 5) | // MSB =TPM2_C0SC[5]=0
								 (0 << 4) | // MSA =TPM2_C0SC[4]=0
								 (1 << 3) | // ELSB=TPM2_C0SC[3]=0
								 (0 << 2);  // ELSA=TPM2_C0SC[2]=0


}


void adcInitModule(void){

	// Set the PORTX_PCRn MUX bit for ADC
	PORTB->PCR[1] |= (0b000 << 8);

	// Enable the clock to ADC0 modules using SIM_SCGC6 register
	SIM->SCGC6 |= (1 << 27);

	// Choose the software trigger using the ADC0_SC2 register.
	ADC0->SC2 &= ~(1 << 6);

	/**
	 * Choose clock rate and resolution using ADC0_CFG1 register
	 */
	// Input Clock Select
	ADC0->CFG1 |= (0b00 << 0); // (Bus clock)

	// Conversion mode selection
	ADC0->CFG1 |= (0b11 << 2); // 16-bit conversion

	// Clock Divide Select
	ADC0->CFG1 |= (0b11 << 5); // (input clock)/8

	// Sample time configuration
	ADC0->CFG1 |= (1 << 10); // Long sample time

	/* ADC0 SC3
	 *
	 * Status and Control Register 3 (ADCx_SC3)
	 *| 31  --  8 |   7  |   6  | 5  4 |  3   |   2  | 1  0 |
	 *|     0	  |  CAL | CALF |  0   | ADC0 | AVGE | AVGS |
	 */
	ADC0->SC3 |= 0b00;//00 4 samples averaged.
}

int main(void) {

	uint32_t result = 0;
	uint32_t step_motor = 0;

	InitTPMPWM();

	adcInitModule();

	while(1){

		/**
		 * ADCH - Input channel select
		 *
		 * Select the ADC input channel using the ADC0_SC1A register
		 *
		 * AD9 is selected as input;
		 */
		ADC0->SC1[0] = (0b01001 << 0);

		// Keep monitoring the end-of-conversion COCO flag in ADC0_SC1A register.
		// When the COCO flag goes HIGH, read the ADC result from the ADC0_RA and save it.
		while((ADC0->SC1[0] & (1 << 7)) == 0);

		/* Ler dados ADC do resultado do registro */
		result = ADC0->R[0];

		// 2^16 = 65535 step size
		step_motor = (result * MAX_VALUE) / 65535;

		TPM1->CONTROLS[0].CnV = step_motor + MIN_VALUE;

	}

	return 0 ;
}
