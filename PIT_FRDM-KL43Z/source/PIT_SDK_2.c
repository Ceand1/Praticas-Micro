///**
// * @file    PIT_SDK.c
// * @brief   Application entry point.
// */
//#include <stdio.h>
//#include "board.h"
//#include "peripherals.h"
//#include "pin_mux.h"
//#include "clock_config.h"
//#include "MKL43Z4.h"
//#include "fsl_debug_console.h"
//#include "fsl_pit.h"
//#include "fsl_gpio.h"
//#include "fsl_port.h"
//
///* TODO: insert other include files here. */
//
///* TODO: insert other definitions and declarations here. */
//
///*
// * Timer in N sec cnt = (CLOCK_GetBusClkFreq) x N
// *
// * Timer in N mSec cnt = (CLOCK_GetBusClkFreq / 1000) X N
// *
// * Timer in N uSec cnt = (CLOCK_GetBusClkFreq / 1000000) x N
// */
//
//#define GET_SEC_COUNT(x) (CLOCK_GetBusClkFreq() * x)
//#define GET_USEC_COUNT(x) ((CLOCK_GetBusClkFreq() * x)/1000000)
//uint32_t timerCnt;
//
//void PIT_IRQHandler(void)
//{
////	GPIO_PortToggle(GPIOD, (1 << 5));
//	if(PIT_GetStatusFlags(PIT, 0) & kPIT_TimerFlag) {
//		PIT_ClearStatusFlags(PIT, 0, kPIT_TimerFlag);
//		GPIO_PortToggle(GPIOD, (1 << 5));
//	} else {
//		PIT_ClearStatusFlags(PIT, 1, kPIT_TimerFlag);
//		GPIO_PortToggle(GPIOE, (1 << 31));
//	}
////	PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);
//}
//
//void DelaySec(uint32_t sec)
//{
//	timerCnt=0;
//	PIT_StartTimer(PIT, kPIT_Chnl_0);
//	while((sec * 100000) > timerCnt);
//	PIT_StopTimer(PIT, kPIT_Chnl_0);
//}
//
////void DelayMsec(uint32_t mSec)
////{
////	timerCnt=0;
////	PIT_StartTimer(PIT, kPIT_Chnl_0);
////	while((mSec * 100) > timerCnt);
////	PIT_StopTimer(PIT, kPIT_Chnl_0);
////
////}
////
////void DelayUSec(uint32_t uSec)
////{
////	timerCnt=0;
////	PIT_StartTimer(PIT, kPIT_Chnl_0);
////	while((uSec / 10) > timerCnt);
////	PIT_StopTimer(PIT, kPIT_Chnl_0);
////}
//
///*
// * @brief   Application entry point.
// */
//int main(void) {
//	// Ativar o clock da PORTA_D e PORTA_E
//	CLOCK_EnableClock(kCLOCK_PortD);
//	CLOCK_EnableClock(kCLOCK_PortE);
//
//	// Ativar o GPIO da PORTA_D_5 e PORTA_E_31
//	PORT_SetPinMux(PORTD, 5, kPORT_MuxAsGpio);
//	PORT_SetPinMux(PORTE, 31, kPORT_MuxAsGpio);
//
//	//Configurando os leds como saída
//	gpio_pin_config_t gpioLed = {kGPIO_DigitalOutput, 0};
//	gpio_pin_config_t gpioLed2 = {kGPIO_DigitalOutput, 0};
//
//	// Configurar GPIO_D pino 5 e GPIO_E pino 31 como saída
//	GPIO_PinInit(GPIOD, 5, &gpioLed);
//	GPIO_PinInit(GPIOE, 31, &gpioLed2);
//
//	//Habilitando clock do PIT
//	CLOCK_EnableClock(kCLOCK_Pit0);
//	pit_config_t pitCfg0 = {0}; //Canal 0
//	pit_config_t pitCfg1 = {0}; //Canal 1
//
//	// Obter configuração padrão do PIT
//	PIT_GetDefaultConfig(&pitCfg0);
//	pitCfg0.enableRunInDebug = true;
//
//	PIT_GetDefaultConfig(&pitCfg1);
//	pitCfg1.enableRunInDebug = true;
//
//	// Initializar o PIT
//	PIT_Init(PIT, &pitCfg0);
//	PIT_Init(PIT, &pitCfg1);
//
//	// Definir o período de tempo do PIT
//	PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, GET_SEC_COUNT(2));
//	PIT_SetTimerPeriod(PIT, kPIT_Chnl_1, GET_SEC_COUNT(6));
//
//	// Ativar o Timer de interrupção
//	PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);
//	PIT_EnableInterrupts(PIT, kPIT_Chnl_1, kPIT_TimerInterruptEnable);
//
//	// Ativar interrução do PIT no NVIC
//	NVIC_EnableIRQ(PIT_IRQn);
//
//	// Iniciar o Timer do PIT
//	PIT_StartTimer(PIT, kPIT_Chnl_0);
//	PIT_StartTimer(PIT, kPIT_Chnl_1);
//
//	while(1) {
//	}
//	return 0;
//}
