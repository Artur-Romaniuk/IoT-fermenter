#include "sntp_init.h"

void start_sntp()
{
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    setenv("TZ", "UTC-2", 1);
    tzset();
    sntp_init();
}