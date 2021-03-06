#if !defined(__NVS_CREDENTIALS_H__)
#define __NVS_CREDENTIALS_H__

#include "nvs_flash.h"
#include "nvs.h"
#include "wifi_init.h"

/**
 * @brief Reads ssid and password from NVS. 
 * 
 * @param ssid Array of 33 chars
 * @param password Array of 65 chars
 * @return esp_err_t 
 */
esp_err_t read_credentials_nvs(uint8_t ssid[33], uint8_t password[65]);
/**
 * @brief Saves ssid and password in NVS.
 * 
 * @param ssid Array of 33 chars
 * @param password Array of 65 chars
 * @return esp_err_t 
 */
esp_err_t save_credentials_nvs(uint8_t ssid[33], uint8_t password[65]);
/**
 * @brief Removes saved ssid and password from NVS.
 * 
 * @return esp_err_t 
 */
esp_err_t erase_credentials_nvs();

#endif // __NVS_CREDENTIALS_H__
