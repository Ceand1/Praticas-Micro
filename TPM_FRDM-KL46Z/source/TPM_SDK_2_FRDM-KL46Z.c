/**
 * @file    TPM.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL46Z4.h"
#include "fsl_debug_console.h"
#include "fsl_tpm.h"
#include "fsl_clock.h"
#include "fsl_port.h"
#include "fsl_gpio.h"

/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

void init_LEDS(void);
void init_TPM0(void);
void init_TPM1(void);

void TPM0_IRQHandler(void){
	TPM_ClearStatusFlags(TPM0, kTPM_TimeOverflowFlag);
	GPIO_TogglePinsOutput(GPIOE, (1 << 29));
}
/*
 * @brief   Application entry point.
 */
int main(void) {

	init_LEDS();
	init_TPM0();

	while(1){}

	return 0 ;
}

void init_LEDS(void) {
	CLOCK_EnableClock(kCLOCK_PortD);
	CLOCK_EnableClock(kCLOCK_PortE);

	PORT_SetPinMux(PORTD, 5, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTE, 29, kPORT_MuxAsGpio);

	gpio_pin_config_t led1 = {kGPIO_DigitalOutput, 0};
	GPIO_PinInit(GPIOD, 5, &led1);

	gpio_pin_config_t led2 = {kGPIO_DigitalOutput, 0};
	GPIO_PinInit(GPIOE, 29, &led2);
}

void init_TPM0(void) {

	CLOCK_SetInternalRefClkConfig(kMCG_IrclkEnable, kMCG_IrcSlow, 0x0u);
	CLOCK_EnableClock(kCLOCK_Tpm0);

	tpm_config_t tpm0_config = {};
	TPM_GetDefaultConfig(&tpm0_config);

	tpm0_config.prescale = kTPM_Prescale_Divide_64;
	TPM_Init(TPM0, &tpm0_config);

	CLOCK_SetTpmClock(3);
	//MOD = 8s * (32768/64) = 
	TPM_SetTimerPeriod(TPM0, 4096);

	TPM_EnableInterrupts(TPM0, kTPM_TimeOverflowInterruptEnable);
	NVIC_EnableIRQ(TPM0_IRQn);

	TPM_StartTimer(TPM0, kTPM_SystemClock);
}







