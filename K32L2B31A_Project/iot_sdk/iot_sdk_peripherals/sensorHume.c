/*! @file : sensorHume.c
 * @author  JAVIER ELIAS TOBON AYUBB
 * @version 1.0.0
 * @date    10/09/2021
 * @brief   Driver para sensor de Humedad
 * @details
 *
*/
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <sensorHume.h>
#include "peripherals.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Private Prototypes
 ******************************************************************************/
//Inicia captura por ADC de voltaje generado por sensor de Humedad
void SenHumeIniciarCap(void);
//-----------------------------------------------------------------------------
//Espera que el ADC obtenga el resultado
void SenHumeEsperarResult(void);


/*******************************************************************************
 * External vars
 ******************************************************************************/


/*******************************************************************************
 * Local vars
 ******************************************************************************/


/*******************************************************************************
 * Private Source Code
 ******************************************************************************/
void SenHumeIniciarCap(void){
	ADC16_SetChannelConfig(SenHume_ADC16_BASE, SenHume_ADC16_CHANNEL_GROUP, & ADC0_channelsConfig[1]);
}

void SenHumeEsperarResult(void){
	while (0U == (kADC16_ChannelConversionDoneFlag & ADC16_GetChannelStatusFlags(SenHume_ADC16_BASE, SenHume_ADC16_CHANNEL_GROUP))){
  	}
}


/*******************************************************************************
 * Public Source Code
 ******************************************************************************/

float SenHumeObtenerDatoRH(void){
	SenHumeIniciarCap();
	SenHumeEsperarResult();
	uint32_t resultadoADC;
	float voltajeADC, HumeRH;
	resultadoADC = ADC16_GetChannelConversionValue(SenHume_ADC16_BASE, SenHume_ADC16_CHANNEL_GROUP);
	voltajeADC = (3.3*resultadoADC)/4095;
	HumeRH = -12.5+(41.667*voltajeADC);
	return(HumeRH);
}
