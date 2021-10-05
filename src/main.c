#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_log.h"
#include "nvs_flash.h"

#include "mdns_init.h"
#include "wifi_init.h"
#include "sntp_init.h"
#include "spiffs_init.h"
#include "server.h"
#include "fermentation_controller.h"
#include "thermometer.h"

// void time_control_task(void *pvParameters)
// {
//     for (;;)
//     {
//         fermentation_t* fermentation = get_fermentation();
//         time_t seconds = time(NULL);
//         if(seconds>=fermentation->start_date);
//     }
// }
void thermometer_task(void *pvParameters)
{
    const char *TAG = "thermometer_task";
    const int ds18b20_gpio = 4;
    const int sample_period = 1000;
    // Create a 1-Wire bus, using the RMT timeslot driver
    OneWireBus *owb;
    ESP_LOGI(TAG, "Initializing OWB");
    owb_rmt_driver_info rmt_driver_info;
    owb = owb_rmt_initialize(&rmt_driver_info, ds18b20_gpio, RMT_CHANNEL_1, RMT_CHANNEL_0);
    DS18B20_Info *ds18b20_info = ds18b20_malloc(); // heap allocation
    thermometer_start(owb, ds18b20_info);
    TickType_t last_wake_time = xTaskGetTickCount();
    for (;;)
    {
        float temperature;
        read_temperature(owb, ds18b20_info, &temperature);
        ESP_LOGI(TAG, "Temperature: %f", temperature);
        vTaskDelayUntil(&last_wake_time, sample_period / portTICK_PERIOD_MS);
    }
}

void app_main()
{
    /**
     * @brief Initializing NVS
     * 
     */
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    /**
     * @brief Initializing MDNS, WiFi and SNTP
     * 
     */
    mdns_start();
    wifi_sta_start();
    sntp_start();

    /**
     * @brief Initialize file storage
     * 
     */
    ESP_ERROR_CHECK(spiffs_start());

    /**
     * @brief Start HTTP server
     * 
     */
    ESP_ERROR_CHECK(server_start());
    // vTaskDelay(1000 / portTICK_PERIOD_MS);
    //TaskHandle_t time_control_task_handle = NULL;
    //xTaskCreate(time_control_task, "time_control", 256, NULL, 4, &time_control_task_handle);
    TaskHandle_t thermometer_task_handle = NULL;
    xTaskCreate(thermometer_task, "thermometer", 2048, NULL, 4, &thermometer_task_handle);
}