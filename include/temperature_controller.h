#if !defined(__TEMPERATURE_CONTROLLER__)
#define __TEMPERATURE_CONTROLLER__

#include "driver/gpio.h"
#include "esp_log.h"

esp_err_t cooler_relay_start();
esp_err_t start_cooling();
esp_err_t stop_cooling();

#endif // __TEMPERATURE_CONTROLLER__
