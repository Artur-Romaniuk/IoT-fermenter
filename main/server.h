#if !defined(__SERVER__)
#define __SERVER__

#include "esp_err.h"
#include "esp_log.h"

#include "esp_vfs.h"
#include "esp_spiffs.h"
#include "esp_http_server.h"

#include "server_handlers.h"

/* Scratch buffer size */
#define SCRATCH_BUFSIZE 8192

/* Max length a file path can have on storage */
#define FILE_PATH_MAX (ESP_VFS_PATH_MAX + CONFIG_SPIFFS_OBJ_NAME_LEN)

typedef struct server_context
{
    char scratch[SCRATCH_BUFSIZE];
} server_context_t;

/**
 * @brief Starts HTTP web server.
 * 
 * @return esp_err_t 
 */
esp_err_t server_start();

#endif // __SERVER__
