#include "fermentation_controller.h"

fermentation_t fermentation;

const char *FERMENTATION_TAG = "server";

ferm_err_t start_fermentation_from_string(char *string, int length)
{
    fermentation.hystheresis = 1;
    if (strstr(string, "stop"))
    {
        stop_fermentation();
    }
    else if (fermentation.is_active == 0)
    {
        fermentation.beer_type = strstr(string, "Lager") != NULL ? LAGER : ALE;
        if (strstr(string, "primary") != NULL)
            fermentation.fermentation_type = PRIMARY;
        else if (strstr(string, "maturation") != NULL)
            fermentation.fermentation_type = MATURATION;
        else
            fermentation.fermentation_type = SECONDARY;
        fermentation.start_date = time(NULL);
        char *dur_begin = strstr(string, "duration=") + strlen("duration=");
        if (dur_begin != NULL)
        {
            fermentation.duration = atoi(dur_begin);
        }
        {
            struct tm *day = localtime(&fermentation.start_date);
            day->tm_yday += fermentation.duration;
            if (day->tm_yday > 365)
            {
                day->tm_yday -= 365;
                day->tm_year++;
            }
            //day->tm_yday -= day->tm_yday > 365 ? 365 : 0;

            fermentation.end_date = *day;
        }

        char *temp_begin = strstr(string, "temperature=") + strlen("temperature=");
        fermentation.temperature = atof(temp_begin);
        start_fermentation();
    }

    return FERM_OK;
}

ferm_err_t start_fermentation()
{
    if (fermentation.is_active == 0)
        fermentation.is_active = 1;
    else
        return FERM_ERROR;
    return FERM_OK;
}

void stop_fermentation()
{
    ESP_LOGI(FERMENTATION_TAG, "Stopping fermentation");
    time_t seconds = time(NULL);
    fermentation.end_date = *localtime(&seconds);
    fermentation.is_active = 0;
}

fermentation_t *get_fermentation()
{
    return &fermentation;
}

float get_fermentation_temperature()
{
    return fermentation.temperature;
}