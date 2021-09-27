#include "server_handlers.h"

const char *SERVER_HANDLER_TAG = "server";

/* Our URI handler function to be called during POST /uri request */
esp_err_t post_handler(httpd_req_t *req)
{
    int total_len = req->content_len;
    int cur_len = 0;
    char *buf = ((server_context_t *)(req->user_ctx))->scratch;
    int received = 0;
    if (total_len >= SCRATCH_BUFSIZE)
    {
        /* Respond with 500 Internal Server Error */
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "content too long");
        return ESP_FAIL;
    }
    while (cur_len < total_len)
    {
        received = httpd_req_recv(req, buf + cur_len, total_len - cur_len);
        //ESP_LOGI(SERVER_HANDLER_TAG, "%d", received);
        if (received <= 0)
        {
            /* Respond with 500 Internal Server Error */
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to post control value");
            return ESP_FAIL;
        }
        cur_len += received;
    }
    buf[total_len] = '\0';
    ESP_LOGI(SERVER_HANDLER_TAG, "%s", buf);
    start_fermentation_from_string(buf, total_len + 1);

    httpd_resp_sendstr(req, "Post control value successfully");

    return ESP_OK;
}

/**
 * @brief Handler for API /info
 * 
 * @param req 
 * @return esp_err_t 
 */
esp_err_t info_get_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "application/json");
    cJSON *cjson = cJSON_CreateObject();
    {
        cJSON *current_time = cJSON_CreateObject();
        time_t seconds = time(NULL);
        struct tm *day = localtime(&seconds);
        cJSON_AddNumberToObject(current_time, "seconds", day->tm_sec);
        cJSON_AddNumberToObject(current_time, "minutes", day->tm_min);
        cJSON_AddNumberToObject(current_time, "hours", day->tm_hour);
        cJSON_AddNumberToObject(current_time, "year", day->tm_year + 1900);
        cJSON_AddNumberToObject(current_time, "month", day->tm_mon + 1);
        cJSON_AddNumberToObject(current_time, "day", day->tm_mday);
        cJSON_AddItemToObject(cjson, "current time", current_time);
    }

    fermentation_t *fermentation = get_fermentation();
    {
        cJSON *start_time = cJSON_CreateObject();
        struct tm *day = localtime(&fermentation->start_date);
        cJSON_AddNumberToObject(start_time, "year", day->tm_year + 1900);
        cJSON_AddNumberToObject(start_time, "day", day->tm_yday);
        cJSON_AddItemToObject(cjson, "start time", start_time);
    }
        {
        cJSON *start_time = cJSON_CreateObject();
        struct tm *day = &fermentation->end_date;
        cJSON_AddNumberToObject(start_time, "year", day->tm_year + 1900);
        cJSON_AddNumberToObject(start_time, "day", day->tm_yday);
        cJSON_AddItemToObject(cjson, "end time", start_time);
    }
    {
        char *beer_type = fermentation->beer_type == LAGER ? "lager" : "ale";
        cJSON_AddStringToObject(cjson, "beer type", beer_type);
    }
    cJSON_AddNumberToObject(cjson, "temperature", fermentation->temperature);
    {
        switch (fermentation->fermentation_type)
        {
        case PRIMARY:
            cJSON_AddStringToObject(cjson, "fermentation type", "primary");
            break;
        case MATURATION:
            cJSON_AddStringToObject(cjson, "fermentation type", "maturation");
            break;
        case SECONDARY:
            cJSON_AddStringToObject(cjson, "fermentation type", "secondary");
            break;
        }
    }
    char *is_active = fermentation->is_active == 0 ? "no" : "yes";
    cJSON_AddStringToObject(cjson, "is active", is_active);

    const char *response = cJSON_Print(cjson);
    httpd_resp_sendstr(req, response);
    free((void *)response);
    cJSON_Delete(cjson);
    return ESP_OK;
}

/**
 * @brief Handler for API /time
 * 
 * @param req 
 * @return esp_err_t 
 */
esp_err_t time_get_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "application/json");
    cJSON *cjson = cJSON_CreateObject();
    time_t seconds = time(NULL);
    struct tm *day = localtime(&seconds);
    cJSON_AddNumberToObject(cjson, "seconds", day->tm_sec);
    cJSON_AddNumberToObject(cjson, "minutes", day->tm_min);
    cJSON_AddNumberToObject(cjson, "hours", day->tm_hour);
    cJSON_AddNumberToObject(cjson, "year", day->tm_year + 1900);
    cJSON_AddNumberToObject(cjson, "month", day->tm_mon + 1);
    cJSON_AddNumberToObject(cjson, "day", day->tm_mday);
    const char *response = cJSON_Print(cjson);
    httpd_resp_sendstr(req, response);
    free((void *)response);
    cJSON_Delete(cjson);
    return ESP_OK;
}

/**
 * @brief Handler for default HTTP GET request 
 * 
 * @param req 
 * @return esp_err_t 
 */
esp_err_t common_get_handler(httpd_req_t *req)
{
    char filepath[FILE_PATH_MAX];
    server_context_t *context = (server_context_t *)req->user_ctx;
    strlcpy(filepath, base_path, sizeof(filepath));

    if (req->uri[strlen(req->uri) - 1] == '/')
    {
        strlcat(filepath, "/index.html", sizeof(filepath));
    }
    else
    {
        strlcat(filepath, req->uri, sizeof(filepath));
    }
    int fd = open(filepath, O_RDONLY, 0);
    if (fd == -1)
    {
        ESP_LOGE(SERVER_HANDLER_TAG, "Failed to open file : %s", filepath);
        /* Respond with 500 Internal Server Error */
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to read existing file");
        return ESP_FAIL;
    }

    set_content_type_from_file(req, filepath);

    char *chunk = context->scratch;
    ssize_t read_bytes;
    do
    {
        /* Read file in chunks into the scratch buffer */
        read_bytes = read(fd, chunk, SCRATCH_BUFSIZE);
        if (read_bytes == -1)
        {
            ESP_LOGE(SERVER_HANDLER_TAG, "Failed to read file : %s", filepath);
        }
        else if (read_bytes > 0)
        {
            /* Send the buffer contents as HTTP response chunk */
            if (httpd_resp_send_chunk(req, chunk, read_bytes) != ESP_OK)
            {
                close(fd);
                ESP_LOGE(SERVER_HANDLER_TAG, "File sending failed!");
                /* Abort sending file */
                httpd_resp_sendstr_chunk(req, NULL);
                /* Respond with 500 Internal Server Error */
                httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to send file");
                return ESP_FAIL;
            }
        }
    } while (read_bytes > 0);
    /* Close file after sending complete */
    close(fd);
    ESP_LOGI(SERVER_HANDLER_TAG, "File sending complete");
    /* Respond with an empty chunk to signal HTTP response completion */
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}