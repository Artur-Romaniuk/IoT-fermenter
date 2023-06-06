#include <sys/cdefs.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"

#include "mdns_init.h"
#include "wifi_init.h"
#include "sntp_init.h"
#include "spiffs_init.h"
#include "server.h"
#include "thermometer.h"
#include "fermentation_controller.h"
#include "temperature_controller.h"


QueueHandle_t temperature_queue = NULL;

void fermentation_control_task(void *pvParameters)
{
    const char *TAG = "fermentation_control_task";
    fermentation_t *fermentation = get_fermentation();
    for (;;)
    {
        time_t seconds = time(NULL);
        struct tm *today = localtime(&seconds);
        if ((today->tm_year == fermentation->end_date.tm_year && today->tm_yday > fermentation->end_date.tm_yday) || (today->tm_year > fermentation->end_date.tm_year))
        {
            fermentation->is_active = false;
        }
        if (fermentation->is_active == true)
        {
            static float temperature = 0.0;
            uint32_t queue_status = xQueueReceive(temperature_queue, &temperature, 1000 / portTICK_PERIOD_MS);
            if(pdFAIL == queue_status){
                ESP_LOGE(TAG, "Queue received temp error");
                stop_cooling();
                continue;
            }
            ESP_LOGI(TAG, "Receiving temperature from the queue %f", temperature);
            if (temperature - fermentation->temperature > fermentation->hystheresis)
            {
                ESP_LOGI(TAG, "Cooling. Current temp: %f. Target: %f", temperature, fermentation->temperature);
                start_cooling();
            }
            else if (fermentation->temperature - temperature > fermentation->hystheresis)
            {
                ESP_LOGI(TAG, "No longer cooling Current temp: %f. Target: %f", temperature, fermentation->temperature);
                stop_cooling();
            }
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
_Noreturn void thermometer_task(void *pvParameters)
{
    const char *TAG = "thermometer_task";
    const gpio_num_t ds18b20_gpio = GPIO_NUM_4;
    const int sample_period = 1000;
    // Create a 1-Wire bus, using the RMT timeslot driver
    OneWireBus *owb = 0;
    ESP_LOGI(TAG, "Initializing OWB");
    owb_rmt_driver_info rmt_driver_info;
    owb = owb_rmt_initialize(&rmt_driver_info, ds18b20_gpio, RMT_CHANNEL_1, RMT_CHANNEL_0);
    DS18B20_Info *ds18b20_info = ds18b20_malloc(); // heap allocation
    bool found_thermometer = thermometer_start(owb, ds18b20_info);

    TickType_t last_wake_time = xTaskGetTickCount();
    for (;;)
    {
        float temperature = 0.f;
        DS18B20_ERROR status = DS18B20_OK;
        if(found_thermometer){
            status = read_temperature(owb, ds18b20_info, &temperature);
        }
        if(!found_thermometer || status != DS18B20_OK){
            if(owb){
                owb->driver->uninitialize(owb);
            }
            if(ds18b20_info){
                ds18b20_free(&ds18b20_info);
            }
            owb = owb_rmt_initialize(&rmt_driver_info, ds18b20_gpio, RMT_CHANNEL_1, RMT_CHANNEL_0);
            ds18b20_info = ds18b20_malloc(); // heap allocation
            found_thermometer = thermometer_start(owb, ds18b20_info);
            vTaskDelayUntil(&last_wake_time, sample_period / portTICK_PERIOD_MS);
            continue;
        }
        xQueueSend(temperature_queue, &temperature, 0);
        ESP_LOGI(TAG, "Sending temperature to the queue: %f", temperature);
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
    /**
     * @brief Setting up GPIO for relay control
     *
     */
    ESP_ERROR_CHECK(cooler_relay_start());
    temperature_queue = xQueueCreate(4, sizeof(float));
    assert(temperature_queue);
    TaskHandle_t fermentation_control_task_handle = NULL;
    xTaskCreate(fermentation_control_task, "fermentation_control", 2048, NULL, 4, &fermentation_control_task_handle);
    TaskHandle_t thermometer_task_handle = NULL;
    xTaskCreate(thermometer_task, "thermometer", 3000, NULL, 5, &thermometer_task_handle);
}
