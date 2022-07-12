/*
 * De acordo com a folha de dados LM35, a inclinação do sensor de temperatura é
 * especificada como 10mv/C (ou seja, 10mv por grau celcius).
 * A inclinação é linear e, portanto, a tensão é aumentada em
 * 10 mv para cada aumento de 1 grau na temperatura.
 *
 * A temperatura pode ser calculada usando ADC de 10 bits
 * com tensão de referência de 3,0 V conforme abaixo:

+------------------------------------+
| Voltage (v)			  AD Reading |
| VREF   			<->   1024       |
| vltg  			<->   ad_reading |
+------------------------------------+

vltg = (VREF * ad_reading) / 1024
vltg = (3.0 * ad_reading) / 1024
vltg (mv) = ((3.0 * ad_reading) / 1024) / 1000

+---------------------------------+
| Temperature (C)        Voltage  |
| 1C  		      <->    10mv     |
| T               <->    vltg(mv) |
+---------------------------------+

T = (vltg(mv) * 1C) / 10mv
T = (((3 * ad_reading) / 1024) / 1000) / 10
T = (adc_reading * 3 * 100) / 1024

A formula para temperatura é:
 ____________
|                                |
| T = (adc_reading * 300) / 1024 |
|____________|

*/

/**
 * @file    lm35.c
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


/* Delay n milliseconds
* The CPU core clock is set to MCGFLLCLK at 41.94 MHz in SystemInit().
*/
void delay(uint16_t cnt){//uint16_t cnt){ //  em mili segundos
	int i, j;
	// 1 instrução é em torno de 1/48000000 = 20ns para ser executada com clock de 48Mhz
	for (j = 0; j < 50; j++){
		for(i = 0; i < cnt; i++);
	}
}


/******************
Constants
*****************/
/* Canal ADC do sensor de temperatura (ADC0_8) */
/* Nota: O canal AD8 é mapeado para o pino PTB0. Por
 * padrão, este pino é configurado como analógico. o
 * clock para PORTB foi habilitado em lcd_init()
 */
#define TEMPERATURE_SENSOR_CHN   (8)

/* Escrevendo 0x1F (31) no registrador ADCH, desabilita o ADC */
#define DISABLE_ADC  (31)

/* Tensão ADC Vref (3,0v) na multiplicação de 100º
    (Consulte a descrição do arquivo acima para este valor)
*/
#define VREF_FACTOR (300)

void scan_temperature(void){
	static uint16_t adc_result;
	static uint16_t adc_result_avg;
	static uint16_t temp;
	static uint8_t loopcntr;

	uint32_t data;
	uint32_t voltage;

	/* Incrementa a cada 50 ms */
	loopcntr++;

	ADC0->SC1[0] = ( (TEMPERATURE_SENSOR_CHN & (8 << 0)) | (ADC0->SC1[0] & ((1 << 6) | (1 << 5)  )
					) );

	while(ADC0->SC2 & (1 << 7)); 	 // Conversion in progress
	while(!(ADC0->SC1[0] & (1 << 7) )); // Run until the conversion is complete
	//while((ADC0->SC1[0] & (1 << 7)) == 0); // add

	/* Obtém o resultado do ADC para o sensor de temperatura */
	adc_result  = adc_result + ADC0->R[0];

	//data = ADC0->R[0];//ADD

	/* Pegue 16 amostras para calcular a temperatura */
	if (loopcntr >= 16){
		adc_result_avg = adc_result >> 4;       /* obtendo o valor médio do ADC*/
		temp = ((adc_result_avg * VREF_FACTOR) >> 10); /* Calcular a temperatura */

		adc_result = 0;
		loopcntr = 0;
	}
	/* Exibe a temperatura */
	printf("Temperatura : %d\n", temp);
	/* Desativa o módulo ADC */
	ADC0->SC1[0] |= (0b11111 << 0);
}



void init_ADC(void){
	int cal_res;

	// Ativar o clock ADC0
	SIM->SCGC6 |= (1 << 27);
	SIM->SCGC5 |= (1 << 10);

	PORTB->PCR[0] = (0b000 << 8);

	do{
		// Calibrate ADC
		cal_res = adc_cal();
	} while (cal_res == -1);

	// Configure ADC
	ADC0->SC1[0] |= (1 << 6);  // interrupção ativada
	ADC0->SC1[0] &= (0 << 5);   // Single-Ended ADC

	ADC0->CFG1 = 0; // Reset register
	ADC0->CFG1 |= 	(0b10 << 2)  |  // 10 bits mode
					(0b01 << 0)  |	// Input Bus Clock/2 (24 Mhz)
					(0b11 << 5)  | 	// Clock divide by 8 (3 Mhz)
					(1 << 7); 		// Low power mode

	ADC0->SC3  &= (0 << 2);
}

int adc_cal(void){
	uint16_t calib;

	ADC0->CFG1 |= (0b11 << 2)  |  	// 16 bits mode
				  (0b01 << 0)  |	// Input Bus Clock divided by 2 (48Mhz / 2)
				  (0b11 << 5);	 	// Clock divide by 8 (3 MHz)

	ADC0->SC3  |= (1 << 2) 		|	// Enable HW average
				  (0b11 << 0)   |	// Set HW average of 32 samples
				  (1 << 7); 	 	// Start calibration process

	while (ADC0->SC3 & (1 << 7)); // Wait for calibration to end

	if (ADC0->SC3 & (1 << 6))	// Check for successful calibration
		return -1;

	calib = 0;
	calib += ADC0->CLPS + ADC0->CLP4 + ADC0->CLP3 +
			     ADC0->CLP2 + ADC0->CLP1 + ADC0->CLP0;
	calib /= 2;
	calib |= 0x8000; 	// Set MSB
	ADC0->PG = calib;

	calib = 0;
	calib += ADC0->CLMS + ADC0->CLM4 + ADC0->CLM3 +
			     ADC0->CLM2 + ADC0->CLM1 + ADC0->CLM0;
	calib /= 2;
	calib |= 0x8000;	// Set MSB
	ADC0->MG = calib;

	return 0;
}


int main(void) {

	// Init ADC
	init_ADC();

	scan_temperature();

	while(1){

		/* Scan Temperatura a cada 50ms */
		scan_temperature();

		delay(5000);
	}

	return 0;
}
