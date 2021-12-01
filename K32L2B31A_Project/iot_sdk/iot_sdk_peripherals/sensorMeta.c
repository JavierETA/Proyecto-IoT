/*! @file : sensorMeta.c
 * @author  JAVIER ELIAS TOBON AYUBB
 * @version 1.0.0
 * @date    10/09/2021
 * @brief   Driver para sensor de metano
 * @details
 *
*/
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <sensorMeta.h>
#include "peripherals.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Private Prototypes
 ******************************************************************************/
//Inicia captura por ADC de voltaje generado por sensor de Metaeratura
 void SenMetaIniciarCap(void);
//-----------------------------------------------------------------------------
//Espera que el ADC obtenga el resultado
 void SenMetaEsperarResult(void);


/*******************************************************************************
 * External vars
 ******************************************************************************/


/*******************************************************************************
 * Local vars
 ******************************************************************************/


/*******************************************************************************
 * Private Source Code
 ******************************************************************************/
void SenMetaIniciarCap(void){
	ADC16_SetChannelConfig(SenMeta_ADC16_BASE, SenMeta_ADC16_CHANNEL_GROUP, & ADC0_channelsConfig[0]);
}

void SenMetaEsperarResult(void){
	while (0U == (kADC16_ChannelConversionDoneFlag & ADC16_GetChannelStatusFlags(SenMeta_ADC16_BASE, SenMeta_ADC16_CHANNEL_GROUP))){
  	}
  }


/*******************************************************************************
 * Public Source Code
 ******************************************************************************/

  float SenMetaObtenerDatoppm(void){
  	SenMetaIniciarCap();
  	SenMetaEsperarResult();
  	uint32_t resultadoADC;
  	float voltajeADC;
  	resultadoADC = ADC16_GetChannelConversionValue(SenMeta_ADC16_BASE, SenMeta_ADC16_CHANNEL_GROUP);
  	voltajeADC = (3.3*resultadoADC)/4095;

   	return(voltajeADC);
  }
