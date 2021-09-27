#if !defined(__NVS_CREDENTIALS_H__)
#define __NVS_CREDENTIALS_H__

#include "nvs_flash.h"
#include "nvs.h"
#include "wifi_init.h"

/**
 * @brief Reads ssid and password from NVS storage. 
 * 
 * @param ssid 
 * @param password 
 * @return esp_err_t 
 */
esp_err_t read_credentials_nvs(uint8_t ssid[33], uint8_t password[65]);
esp_err_t save_credentials_nvs(uint8_t ssid[33], uint8_t password[65]);

#endif // __NVS_CREDENTIALS_H__
