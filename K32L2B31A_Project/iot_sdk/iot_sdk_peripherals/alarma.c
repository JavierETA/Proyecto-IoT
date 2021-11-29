/*! @file : alarma.c
 * @author  Camilo Mozo
 * @version 1.0.0
 * @date    29/11/2021
 * @brief   Driver para 
 * @details
 *
*/
/* ------- INCLUDES -------- */
#define "alarma.h"
#define "fls_gpio.h"

/* ------- DEFINITIONS ------*/


/* ------- PRIVATE PROTOTYPES ----------- */
void activarAlarma(void);
void apagarAlarma(void);

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

void controlAlarma(char control){

	if (control == "activar") {
		activarAlarma();
	}else if (control == "desactivar"){
		apagarAlarma();
	}
}
