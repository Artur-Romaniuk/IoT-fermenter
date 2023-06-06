//
// Created by artur on 04.06.23.
//

#include "thermometer.h"

#define DS18B20_RESOLUTION (DS18B20_RESOLUTION_12_BIT)

static const char *TAG = "thermometer";

bool thermometer_start(OneWireBus *owb, DS18B20_Info *ds18b20_info)
{
    if(!owb || !ds18b20_info){
        return false;
    }
    ESP_LOGI(TAG, "Starting thermometer");

    ESP_LOGI(TAG, "Enabling CRC check");
    owb_use_crc(owb, true); // enable CRC check for ROM code

    // Find connected device
    printf("Find device:\n");
    int num_devices = 0;
    OneWireBus_SearchState search_state = {};
    bool found = false;
    owb_status status = owb_search_first(owb, &search_state, &found);
    if(status != OWB_STATUS_OK || !found){
        ESP_LOGW(TAG, "Thermometer not found!");
        return false;
    }
    if (found)
    {
        num_devices++;
        char rom_code_s[17];
        owb_string_from_rom_code(search_state.rom_code, rom_code_s, sizeof(rom_code_s));
        printf("  %d : %s\n", num_devices, rom_code_s);
    }


    if (num_devices == 1)
    {
        // For a single device only:
        OneWireBus_ROMCode rom_code;
        owb_status status = owb_read_rom(owb, &rom_code);
        if (status == OWB_STATUS_OK)
        {
            char rom_code_s[OWB_ROM_CODE_STRING_LENGTH];
            owb_string_from_rom_code(rom_code, rom_code_s, sizeof(rom_code_s));
            printf("Single device %s present\n", rom_code_s);
        }
        else
        {
            printf("An error occurred reading ROM code: %d", status);
        }
    }

    if (num_devices == 1)
    {
        printf("Single device optimizations enabled\n");
        ds18b20_init_solo(ds18b20_info, owb); // only one device on bus
    }
    ds18b20_use_crc(ds18b20_info, true); // enable CRC check on all reads
    ds18b20_set_resolution(ds18b20_info, DS18B20_RESOLUTION);
    bool parasitic_power = false;
    ds18b20_check_for_parasite_power(owb, &parasitic_power);
    if (parasitic_power)
    {
        printf("Parasitic-powered devices detected");
    }
    return true;
}

DS18B20_ERROR read_temperature(OneWireBus *owb, DS18B20_Info *ds18b20_info, float *temperature)
{
    if(!owb || !ds18b20_info){
        printf("Got null parameter");
        return DS18B20_ERROR_NULL;
    }
    ds18b20_convert_all(owb);
    ds18b20_wait_for_conversion(ds18b20_info);
    // Read the results immediately after conversion otherwise it may fail
    // (using printf before reading may take too long)
    float reading = 0;
    DS18B20_ERROR status = DS18B20_OK;
    status = ds18b20_read_temp(ds18b20_info, &reading);
    if (status == DS18B20_OK)
        *temperature = reading;

    return status;
}
