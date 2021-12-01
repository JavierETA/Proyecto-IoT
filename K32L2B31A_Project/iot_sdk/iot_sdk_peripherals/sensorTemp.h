/*! @file : sensorTemp.h
 * @author  JAVIER ELIAS TOBON AYUBB
 * @version 1.0.0
 * @date    30/11/2021
 * @brief   Driver para sensor de temperatura
 * @details
 *
 */
#ifndef IOT_SDK_PERIPHERALS_SENSORTEMP_H_
#define IOT_SDK_PERIPHERALS_SENSORTEMP_H_
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "peripherals.h"

/*!
 * @addtogroup X
 * @{
 */
/*!
 * @addtogroup X
 * @{
 */
/*******************************************************************************
 * Public Definitions
 ******************************************************************************/
#define SenTemp_ADC16_BASE          ADC0
#define SenTemp_ADC16_CHANNEL_GROUP 0U
#define SenTemp_ADC16_USER_CHANNEL  13U
/*******************************************************************************
 * External vars
 ******************************************************************************/

/*******************************************************************************
 * Public vars
 ******************************************************************************/

/*******************************************************************************
 * Public Prototypes
 ******************************************************************************/
float SenTempObtenerDatoCenti(void);
/** @} */ // end of X group
/** @} */ // end of X group

#endif /* IOT_SDK_PERIPHERALS_SENSORTEMP_H_ */
