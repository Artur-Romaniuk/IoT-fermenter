//
// Created by artur on 04.06.23.
//

#ifndef MAIN_SPIFFS_INIT_H
#define MAIN_SPIFFS_INIT_H

#include "esp_log.h"
#include "esp_system.h"
#include "esp_spiffs.h"
#include "esp_err.h"

#define SPIFFS_TAG "spiffs"
extern const char *base_path;
/**
 * @brief Initializes SPIFFS for accessing files in /data.
 *
 * @return esp_err_t
 */
esp_err_t spiffs_start(void);

#endif //MAIN_SPIFFS_INIT_H
