/*! @file : sensor_temp.c
 * @author  JAVIER ELIAS TOBON AYUBB
 * @version 1.0.0
 * @date    10/09/2021
 * @brief   Driver para sensor de temperatura
 * @details
 *
*/
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <sensor2.h>
#include "peripherals.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Private Prototypes
 ******************************************************************************/
//Inicia captura por ADC de voltaje generado por sensor de temperatura
 void SenTempIniciarCap2(void);
//-----------------------------------------------------------------------------
//Espera que el ADC obtenga el resultado
 void SenTempEsperarResult2(void);


/*******************************************************************************
 * External vars
 ******************************************************************************/


/*******************************************************************************
 * Local vars
 ******************************************************************************/


/*******************************************************************************
 * Private Source Code
 ******************************************************************************/
 void SenTempIniciarCap2(void){
      ADC16_SetChannelConfig(SenTemp_ADC16_BASE2, SenTemp_ADC16_CHANNEL_GROUP2, & ADC0_channelsConfig[0]);

  }

  void SenTempEsperarResult2(void){
  	while (0U == (kADC16_ChannelConversionDoneFlag & ADC16_GetChannelStatusFlags(SenTemp_ADC16_BASE2, SenTemp_ADC16_CHANNEL_GROUP2))){
  	}
  }


/*******************************************************************************
 * Public Source Code
 ******************************************************************************/

  float SenTempObtenerDatoCenti2(void){
  	SenTempIniciarCap2();
  	SenTempEsperarResult2();
  	uint32_t resultadoADC;
  	float voltajeADC;
  	resultadoADC = ADC16_GetChannelConversionValue(SenTemp_ADC16_BASE2, SenTemp_ADC16_CHANNEL_GROUP2);
  	voltajeADC = (3.3*resultadoADC)/4095;

   	return(voltajeADC);
  }
