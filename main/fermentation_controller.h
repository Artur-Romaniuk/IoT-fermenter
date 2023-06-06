#if !defined(__FERMENTATION_CONTROLLER__)
#define __FERMENTATION_CONTROLLER__

#include "string.h"
#include "time.h"
#include "esp_err.h"
#include "esp_log.h"

typedef enum
{
    ALE,
    LAGER
} beer_type_t;

typedef enum
{
    PRIMARY,
    MATURATION,
    SECONDARY
} fermentation_type_t;

typedef enum
{
    FERM_OK,
    FERM_ERROR
} ferm_err_t;

typedef struct
{
    int is_active;
    beer_type_t beer_type;
    fermentation_type_t fermentation_type;
    int duration;
    float temperature;
    float hystheresis;
    time_t start_date;
    struct tm end_date;
} fermentation_t;

ferm_err_t start_fermentation_from_string(char *string, int length);
fermentation_t *get_fermentation();
ferm_err_t start_fermentation();
void stop_fermentation();

#endif // __FERMENTATION_CONTROLLER__
