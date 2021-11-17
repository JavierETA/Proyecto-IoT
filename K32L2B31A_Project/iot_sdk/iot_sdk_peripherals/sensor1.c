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
#include <sensor1.h>
#include "peripherals.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Private Prototypes
 ******************************************************************************/
//Inicia captura por ADC de voltaje generado por sensor de temperatura
 void SenTempIniciarCap1(void);
//-----------------------------------------------------------------------------
//Espera que el ADC obtenga el resultado
 void SenTempEsperarResult1(void);


/*******************************************************************************
 * External vars
 ******************************************************************************/


/*******************************************************************************
 * Local vars
 ******************************************************************************/


/*******************************************************************************
 * Private Source Code
 ******************************************************************************/
 void SenTempIniciarCap1(void){
      ADC16_SetChannelConfig(SenTemp_ADC16_BASE1, SenTemp_ADC16_CHANNEL_GROUP1, & ADC0_channelsConfig[1]);

  }

  void SenTempEsperarResult1(void){
  	while (0U == (kADC16_ChannelConversionDoneFlag & ADC16_GetChannelStatusFlags(SenTemp_ADC16_BASE1, SenTemp_ADC16_CHANNEL_GROUP1))){
  	}
  }


/*******************************************************************************
 * Public Source Code
 ******************************************************************************/

  float SenTempObtenerDatoCenti1(void){
  	SenTempIniciarCap1();
  	SenTempEsperarResult1();
  	uint32_t resultadoADC;
  	float voltajeADC;
  	resultadoADC = ADC16_GetChannelConversionValue(SenTemp_ADC16_BASE1, SenTemp_ADC16_CHANNEL_GROUP1);
  	voltajeADC = (3.3*resultadoADC)/4095;

   	return(voltajeADC);
  }
