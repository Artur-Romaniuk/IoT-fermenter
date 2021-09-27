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

// void time_control_task(void *pvParameters)
// {
//     for (;;)
//     {
//         fermentation_t* fermentation = get_fermentation();
//         time_t seconds = time(NULL);
//         if(seconds>=fermentation->start_date);
//     }
// }

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
    mdns_init();
    wifi_init_sta();
    start_sntp();

    /**
     * @brief Initialize file storage
     * 
     */
    ESP_ERROR_CHECK(init_spiffs());

    /**
     * @brief Start HTTP server
     * 
     */
    ESP_ERROR_CHECK(start_server());

    //TaskHandle_t time_control_task_handle = NULL;
    //xTaskCreate(time_control_task, "time_control", 256, NULL, 4, &time_control_task_handle);
}