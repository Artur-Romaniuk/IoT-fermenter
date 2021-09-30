#if !defined(__SPIFFS_INIT__)
#define __SPIFFS_INIT__

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

#endif // __SPIFFS_INIT__