FILE(GLOB_RECURSE app_sources ./*.*)

idf_component_register(SRCS ${app_sources} PRIV_REQUIRES esp32-owb esp32-ds18b20 nvs_flash esp_event esp_wifi esp_http_server spiffs mdns json)
