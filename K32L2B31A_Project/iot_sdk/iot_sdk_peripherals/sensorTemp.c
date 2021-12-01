/*! @file : sensorTemp.c
 * @author  JAVIER ELIAS TOBON AYUBB
 * @version 1.0.0
 * @date    30/11/2021
 * @brief   Driver para sensor de temperatura
 * @details
 *
*/
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <sensorTemp.h>
#include "peripherals.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Private Prototypes
 ******************************************************************************/
//Inicia captura por ADC de voltaje generado por sensor de temperatura
void SenTempIniciarCap(void);
//-----------------------------------------------------------------------------
//Espera que el ADC obtenga el resultado
void SenTempEsperarResult(void);
/*******************************************************************************
 * External vars
 ******************************************************************************/


/*******************************************************************************
 * Local vars
 ******************************************************************************/


/*******************************************************************************
 * Private Source Code
 ******************************************************************************/
void SenTempIniciarCap(void){
	ADC16_SetChannelConfig(SenTemp_ADC16_BASE, SenTemp_ADC16_CHANNEL_GROUP, & ADC0_channelsConfig[2]);
}

void SenTempEsperarResult(void){
	while (0U == (kADC16_ChannelConversionDoneFlag & ADC16_GetChannelStatusFlags(SenTemp_ADC16_BASE, SenTemp_ADC16_CHANNEL_GROUP))){
   	}
}


/*******************************************************************************
 * Public Source Code
 ******************************************************************************/
float SenTempObtenerDatoCenti(void){
	SenTempIniciarCap();
	SenTempEsperarResult();
	uint32_t resultadoADC;
	float voltajeADC, TempC;
	resultadoADC = ADC16_GetChannelConversionValue(SenTemp_ADC16_BASE, SenTemp_ADC16_CHANNEL_GROUP);
	voltajeADC = (3.3*resultadoADC)/4095;
	TempC = -66.875+(72.917*voltajeADC);
	return(TempC);
}
