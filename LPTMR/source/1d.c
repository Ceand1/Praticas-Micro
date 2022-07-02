//*
// * @file    LPTMR_SDK.c
// * @brief   Application entry point.
//
//#include <stdio.h>
//#include "board.h"
//#include "peripherals.h"
//#include "pin_mux.h"
//#include "clock_config.h"
//#include "MKL43Z4.h"
//#include "fsl_debug_console.h"
//#include "fsl_gpio.h"
//#include "fsl_port.h"
//#include "fsl_lptmr.h"
//
// TODO: insert other include files here.
//
// TODO: insert other definitions and declarations here.
//
//void LPTMR0_IRQHandler(void) {
//
//	GPIO_PortToggle(GPIOE, (1 << 31));
//	LPTMR_ClearStatusFlags(LPTMR0, kLPTMR_TimerCompareFlag);
//}
//
//
// * @brief   Application entry point.
//
//int main(void) {
//	lptmr_config_t lptmrCfg = {0};
//
//	gpio_pin_config_t gpioLed = {
//				kGPIO_DigitalOutput,
//				1
//	};
//
//	// Ativando o clock da PORTA_B
//	CLOCK_EnableClock(kCLOCK_PortE);
//
//	// Ativando PORTA_B_18 como GPIO
//	PORT_SetPinMux(PORTE, 31, kPORT_MuxAsGpio);
//
//	//Configurando GPIO_C_9 como Output
//	GPIO_PinInit(GPIOE, 31, &gpioLed);
//
//	//Obtendo a configuração padrão do LPTMR
//	LPTMR_GetDefaultConfig(&lptmrCfg);
//
//	lptmrCfg.bypassPrescaler = true;
//	lptmrCfg.value = 1;
//
//	//Inicializando o LPTMR
//	LPTMR_Init(LPTMR0, &lptmrCfg);
//
//	//Registrador CMR LPTMR para 1000 em 1s Timer
//	LPTMR_SetTimerPeriod(LPTMR0, 1000);
//
//	// Interrução da LPTMR
//	LPTMR_EnableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
//
//	//ativando a interrução para o LPTMR
//	NVIC_EnableIRQ(LPTMR0_IRQn);
//
//	//iniciando o Timer
//	LPTMR_StartTimer(LPTMR0);
//
//	while(1){
//
//	}
//
//    return 0 ;
//}
