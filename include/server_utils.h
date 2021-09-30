#if !defined(__SERVER_UTILS__)
#define __SERVER_UTILS__

#include "esp_err.h"
#include "esp_log.h"
#include "esp_vfs.h"
#include "esp_spiffs.h"
#include "esp_http_server.h"

/**
 * @brief Check file extension
 * 
 * @param filename pointer to C-style string
 * @param base extension that we are looking for
 * @return int 
 */
int check_file_extension(const char *filename, const char *base);
/**
 * @brief Set HTTP response content type according to file extension
 * 
 * @param req 
 * @param filepath 
 * @return esp_err_t 
 */
esp_err_t set_content_type_from_file(httpd_req_t *req, const char *filepath);

#endif // __SERVER_UTILS__
