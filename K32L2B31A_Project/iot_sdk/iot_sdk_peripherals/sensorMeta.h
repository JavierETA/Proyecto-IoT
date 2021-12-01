/*! @file : sensorMeta.h
 * @author  JAVIER ELIAS TOBON AYUBB
 * @version 1.0.0
 * @date    10/09/2021
 * @brief   Driver para sensor de metano
 * @details
 *
 */
#ifndef IOT_SDK_PERIPHERALS_SENSORMETA_H_
#define IOT_SDK_PERIPHERALS_SENSORMETA_H_
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "peripherals.h"

/*!
 * @addtogroup periferico
 * @{
 */
/*!
 * @addtogroup sensor de Metano
 * @{
 */
/*******************************************************************************
 * Public Definitions
 ******************************************************************************/
#define SenMeta_ADC16_BASE          ADC0
#define SenMeta_ADC16_CHANNEL_GROUP 0U
#define SenMeta_ADC16_USER_CHANNEL  9U
/*******************************************************************************
 * External vars
 ******************************************************************************/

/*******************************************************************************
 * Public vars
 ******************************************************************************/

/*******************************************************************************
 * Public Prototypes
 ******************************************************************************/
 //Returna el resultado ADC del sensor de Metano
 float SenMetaObtenerDatoppm(void);
/** @} */ // end of sensor de Metatano group
/** @} */ // end of periferico group

#endif /* IOT_SDK_PERIPHERALS_SENSORMETA_H_ */
