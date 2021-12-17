/*! @file : alarma.c
 * @author  Camilo Mozo
 * @version 1.0.0
 * @date    29/11/2021
 * @brief   Driver para 
 * @details
 *
*/
/* ------- INCLUDES -------- */
#include "alarma.h"
#include "fsl_gpio.h"

/* ------- DEFINITIONS ------*/


/* ------- PRIVATE PROTOTYPES ----------- */


/* ------- EXTERNAL VARIABLES ----------- */


/* ------- LOCAL VARIABLES -------------- */


/* ------- PRIVATE SOURCE CODE ---------- */


/* ------- PUBLIC SOURCE CODE ----------- */
 
void activarAlarma(void){
	GPIO_PinWrite(GPIOD, 6, 1);
}

void apagarAlarma(void){
	GPIO_PinWrite(GPIOD, 6, 0);
}
