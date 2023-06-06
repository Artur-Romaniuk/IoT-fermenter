//
// Created by artur on 04.06.23.
//

#include "sntp_init.h"

void sntp_start()
{
    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
    esp_sntp_setservername(0, "pool.ntp.org");
    setenv("TZ", "UTC-2", 1);
    tzset();
    esp_sntp_init();
}
