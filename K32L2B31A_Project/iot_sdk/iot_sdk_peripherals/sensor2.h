/*! @file : sensor_temp.h
 * @author  JAVIER ELIAS TOBON AYUBB
 * @version 1.0.0
 * @date    10/09/2021
 * @brief   Driver para sensor de temperatura
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
 * @addtogroup sensor de temperatura
 * @{
 */
/*******************************************************************************
 * Public Definitions
 ******************************************************************************/
#define SenTemp_ADC16_BASE2          ADC0
#define SenTemp_ADC16_CHANNEL_GROUP2 0U
#define SenTemp_ADC16_USER_CHANNEL2  13U
/*******************************************************************************
 * External vars
 ******************************************************************************/

/*******************************************************************************
 * Public vars
 ******************************************************************************/

/*******************************************************************************
 * Public Prototypes
 ******************************************************************************/
 //Returna el resultado ADC del sensor de temperatura
 float SenTempObtenerDatoCenti2(void);
/** @} */ // end of sensor de temperatura group
/** @} */ // end of periferico group

#endif /* IOT_SDK_PERIPHERALS_SENSOR_TEMP1_H_ */
