#if !defined(__MDNS_INIT__)
#define __MDNS_INIT__

#include "esp_log.h"
#include "mdns.h"

#define MDNS_HOST_NAME "FermIOT"
#define MDNS_INSTANCE "esp home web server"

void mdns_start(void);

#endif // __MDNS_INIT__
