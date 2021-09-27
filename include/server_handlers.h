#if !defined(__SERVER_HANDLERS)
#define __SERVER_HANDLERS

#include <stdio.h>
#include <string.h>
#include "cJSON.h"

#include "esp_err.h"
#include "esp_log.h"
#include "esp_vfs.h"
#include "esp_spiffs.h"
#include "esp_http_server.h"
#include <fcntl.h>

#include "server.h"
#include "server_utils.h"
#include "spiffs_init.h"
#include "fermentation_controller.h"

esp_err_t info_get_handler(httpd_req_t *req);
esp_err_t time_get_handler(httpd_req_t *req);
esp_err_t common_get_handler(httpd_req_t *req);
esp_err_t post_handler(httpd_req_t *req);

#endif // __SERVER_HANDLERS
