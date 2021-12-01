/*! @file : sensorHume.h
 * @author  JAVIER ELIAS TOBON AYUBB
 * @version 1.0.0
 * @date    10/09/2021
 * @brief   Driver para sensor de humedad
 * @details
 *
 */
#ifndef IOT_SDK_PERIPHERALS_SENSOR_TEMP1_H_
#define IOT_SDK_PERIPHERALS_SENSOR_TEMP1_H_
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "peripherals.h"

/*!
 * @addtogroup periferico
 * @{
 */
/*!
 * @addtogroup sensor de humedad
 * @{
 */
/*******************************************************************************
 * Public Definitions
 ******************************************************************************/
#define SenHume_ADC16_BASE          ADC0
#define SenHume_ADC16_CHANNEL_GROUP 0U
#define SenHume_ADC16_USER_CHANNEL  12U
/*******************************************************************************
 * External vars
 ******************************************************************************/

/*******************************************************************************
 * Public vars
 ******************************************************************************/

/*******************************************************************************
 * Public Prototypes
 ******************************************************************************/
 //Returna el resultado ADC del sensor de humedad
 float SenHumeObtenerDatoRH(void);
/** @} */ // end of sensor de humedad group
/** @} */ // end of periferico group

#endif /* IOT_SDK_PERIPHERALS_SENSOR_TEMP1_H_ */
