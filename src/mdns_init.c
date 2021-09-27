#include "mdns_init.h"

void initialise_mdns(void)
{
    ESP_ERROR_CHECK(mdns_init());
    ESP_ERROR_CHECK(mdns_hostname_set(MDNS_HOST_NAME));
    ESP_ERROR_CHECK(mdns_instance_name_set(MDNS_INSTANCE));

    mdns_txt_item_t serviceTxtData[] = {
        {"board", "esp32"},
        {"path", "/"}};

    ESP_ERROR_CHECK(mdns_service_add("ESP32-WebServer", "_http", "_tcp", 80, serviceTxtData, sizeof(serviceTxtData) / sizeof(serviceTxtData[0])));
}