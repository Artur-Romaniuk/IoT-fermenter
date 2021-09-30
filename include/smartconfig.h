#if !defined(__SMARTCONFIG_H__)
#define __SMARTCONFIG_H__

#include "wifi_init.h"
/**
 * @brief FreeRTOS task for managing SmartConfig.
 * 
 * @param parm 
 */
void smartconfig_task(void *parm);
/**
 * @brief Starts SmartConfig.
 * 
 */
void smartconfig_start();
/**
 * @brief Stops SmartConfig.
 * 
 */
void smartconfig_stop();

#endif // __SMARTCONFIG_H__
