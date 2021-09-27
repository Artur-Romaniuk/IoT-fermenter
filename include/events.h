#if !defined(__EVENTS__)
#define __EVENTS__

#include "string.h"
#include "esp_log.h"
#include "esp_event.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "wifi_init.h"
#include "smartconfig.h"

void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

#endif // __EVENTS__
