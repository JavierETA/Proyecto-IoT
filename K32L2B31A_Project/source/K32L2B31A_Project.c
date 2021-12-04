/*
* @author  JAVIER ELIAS TOBON AYUBB
 * @version 1.0.0
 * @date    ${date}
 * @brief   Driver para
 * @details
 *
*/
/**
 * @file    K32L2B31A_Project.c
 * @brief   Application entry point.
 */
#include <sensorHume.h>
#include <sensorMeta.h>
#include <sensorTemp.h>
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "K32L2B31A.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */

#include "irq_lptmr0.h"
#include "irq_lpuart0.h"
#include "modem.h"
#include "sensorHume.h"
#include "sensorMeta.h"
#include "sensorTemp.h"

/* TODO: insert other definitions and declarations here. */

#define SIZE_BUFFER_COMANDO	105
/*******************************************************************************
 * Private Prototypes
 ******************************************************************************/

/*******************************************************************************
 * External vars
 ******************************************************************************/

/*******************************************************************************
 * Local vars
 ******************************************************************************/
volatile uint32_t segAct=0;
/*******************************************************************************
 * Private Source Code
 ******************************************************************************/

// Inicializa Alarma
void Alarma_Init(void){
	segAct = lptmr0_irqCounter;
}

// retorna el tiempo futuro de la alarma
uint32_t Alarma_Set(uint32_t time2Wait){
	segAct = lptmr0_irqCounter;
	return (segAct + time2Wait);
}

// retorna si la alarma se cumplio
char Alarma_Elapsed(uint32_t time2Test){
	segAct = lptmr0_irqCounter;
	if(segAct >= time2Test) return true;
	else return false;
}

#define Timer_Init() LPTMR_StartTimer(LPTMR0)

/* Init board hardware. */
void BOARD_Init(void){
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif
}

//  main
int main(void) {
	BOARD_Init();
    Timer_Init();
    Modem_Init();
    Alarma_Init();
    int var;
    while(1) {
//    	Modem_Task_Run();
    	printf("%.2f --- %.2f ---- %.2f\r\n",
    			SenHumeObtenerDatoRH(), SenTempObtenerDatoCenti(), SenMetaObtenerDatoppm());
    	for (var = 0; var < 0xFFFFFF; ++var) {

		}
    }
    return 0 ;
}
