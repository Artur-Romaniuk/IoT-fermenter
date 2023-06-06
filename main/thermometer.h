//
// Created by artur on 04.06.23.
//

#ifndef MAIN_THERMOMETER_H
#define MAIN_THERMOMETER_H


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"

#include "../components/esp32-ds18b20/include/ds18b20.h"
#include "../components/esp32-owb/include/owb.h"
#include "../components/esp32-owb/include/owb_rmt.h"

bool thermometer_start(OneWireBus *owb, DS18B20_Info *ds18b20_info);
DS18B20_ERROR read_temperature(OneWireBus *owb, DS18B20_Info *ds18b20_info, float *temperature);

#endif //MAIN_THERMOMETER_H
