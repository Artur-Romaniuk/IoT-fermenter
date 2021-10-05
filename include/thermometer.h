#if !defined(__THERMOMETER_H__)
#define __THERMOMETER_H__

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "owb.h"
#include "owb_rmt.h"
#include "ds18b20.h"

void thermometer_start(OneWireBus *owb, DS18B20_Info *ds18b20_info);
DS18B20_ERROR read_temperature(OneWireBus *owb, DS18B20_Info *ds18b20_info, float *temperature);
#endif // __THERMOMETER_H__
