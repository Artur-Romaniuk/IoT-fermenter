#if !defined(__WIFI_INIT__)
#define __WIFI_INIT__

#include "esp_event.h"
#include "esp_wifi.h"
#include "esp_smartconfig.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_system.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "events.h"

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1
#define SC_ESPTOUCH_DONE_BIT BIT2
#define SC_STARTED BIT3

EventGroupHandle_t s_wifi_event_group;

/**
 * @brief Starts wifi connection procedure.
 * 
 */
void wifi_sta_start(void);

#endif // __WIFI_INIT__
