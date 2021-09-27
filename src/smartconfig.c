#include "smartconfig.h"

static const char *TAG = "smartconfig";

void smartconfig_task(void *parm)
{
    xEventGroupSetBits(s_wifi_event_group, SC_STARTED);
    EventBits_t uxBits;
    ESP_ERROR_CHECK(esp_smartconfig_set_type(SC_TYPE_ESPTOUCH));
    smartconfig_start_config_t cfg = SMARTCONFIG_START_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_smartconfig_start(&cfg));
    while (1)
    {
        uxBits = xEventGroupWaitBits(s_wifi_event_group, WIFI_CONNECTED_BIT | SC_ESPTOUCH_DONE_BIT, true, false, portMAX_DELAY);
        ESP_LOGI(TAG, "SmartConfig got event group bit");
        if (uxBits & WIFI_CONNECTED_BIT)
        {
            ESP_LOGI(TAG, "WiFi Connected to ap");
        }
        else if (uxBits & SC_ESPTOUCH_DONE_BIT)
        {
            ESP_LOGI(TAG, "smartconfig over");
            xEventGroupClearBits(s_wifi_event_group, SC_STARTED);
            esp_smartconfig_stop();
            vTaskDelete(NULL);
        }
    }
}