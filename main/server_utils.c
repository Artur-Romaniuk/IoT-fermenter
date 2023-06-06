#include "server_utils.h"

int check_file_extension(const char *filename, const char *base)
{
    return (strcasecmp(&filename[strlen(filename) - strlen(base)], base) == 0);
}

esp_err_t set_content_type_from_file(httpd_req_t *req, const char *filepath)
{
    const char *type = "text/plain";
    if (check_file_extension(filepath, ".html"))
    {
        type = "text/html";
    }
    else if (check_file_extension(filepath, ".js"))
    {
        type = "application/javascript";
    }
    else if (check_file_extension(filepath, ".css"))
    {
        type = "text/css";
    }
    else if (check_file_extension(filepath, ".png"))
    {
        type = "image/png";
    }
    else if (check_file_extension(filepath, ".ico"))
    {
        type = "image/x-icon";
    }
    else if (check_file_extension(filepath, ".svg"))
    {
        type = "text/xml";
    }
    return httpd_resp_set_type(req, type);
}
