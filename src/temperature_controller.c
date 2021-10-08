#include "temperature_controller.h"

static const char *TAG = "relay";
static const gpio_num_t cooler_gpio = GPIO_NUM_27;

esp_err_t cooler_relay_start()
{
    ESP_LOGI(TAG, "Setting up GPIO");
    //zero-initialize the config structure.
    gpio_config_t io_conf = {};
    //disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = BIT(cooler_gpio);
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    ESP_LOGI(TAG, "Setting up GPIO complete");
    return gpio_config(&io_conf);
}

esp_err_t start_cooling()
{
    ESP_LOGI(TAG, "Starting cooling");
    return gpio_set_level(cooler_gpio, 1);
}
esp_err_t stop_cooling()
{
    ESP_LOGI(TAG, "Stoping cooling");
    return gpio_set_level(cooler_gpio, 0);
}
