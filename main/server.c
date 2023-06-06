#include "server.h"

const char *SERVER_TAG = "server";

/* Function to start the  server */
esp_err_t server_start()
{
    server_context_t *context = calloc(1, sizeof(server_context_t));

    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.uri_match_fn = httpd_uri_match_wildcard;

    ESP_LOGI(SERVER_TAG, "Starting HTTP Server");
    httpd_start(&server, &config);

    /**
     * @brief API for getting system info
     * 
     */
    httpd_uri_t info_get_uri = {
        .uri = "/api/info",
        .method = HTTP_GET,
        .handler = info_get_handler,
        .user_ctx = context};
    httpd_register_uri_handler(server, &info_get_uri);

    /**
     * @brief API for getting system time
     * 
     */
    httpd_uri_t time_get_uri = {
        .uri = "/api/time",
        .method = HTTP_GET,
        .handler = time_get_handler,
        .user_ctx = context};
    httpd_register_uri_handler(server, &time_get_uri);

    /**
     * @brief URI handler structure for POST /uri 
     * 
     */
    httpd_uri_t uri_post = {
        .uri = "/*",
        .method = HTTP_POST,
        .handler = post_handler,
        .user_ctx = context};
    httpd_register_uri_handler(server, &uri_post);

    /**
     * @brief URI handler for getting server files (including default index.html)
     * 
     */
    httpd_uri_t common_get_uri = {
        .uri = "/*",
        .method = HTTP_GET,
        .handler = common_get_handler,
        .user_ctx = context};
    httpd_register_uri_handler(server, &common_get_uri);

    return ESP_OK;
}