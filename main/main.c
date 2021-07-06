#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <hx711.h>
#include <esp_wifi.h>


#define PD_SCK_GPIO 12
#define DOUT_GPIO   13


void test(void *pvParameters)
{
    // esp_netif_init();
    // printf("netif intialised \n");
    // esp_event_loop_create_default();
    // printf("event loop intialised \n");
    // esp_netif_create_default_wifi_sta();
    // printf("create station \n");
    // wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    // esp_wifi_init(&cfg);
    // printf("intialise wifi \n");
    // esp_wifi_set_mode(WIFI_MODE_STA);
    // printf("Set mode to station \n");
    // esp_wifi_start();
    // printf("Wifi started");
    // esp_wifi_connect();
    // printf("Starting wifi connection");
    // esp_wifi_disconnect();
    //  esp_wifi_stop();
    //  esp_wifi_deinit();
        hx711_t dev = {
        .dout = DOUT_GPIO,
        .pd_sck = PD_SCK_GPIO,
        .gain = HX711_GAIN_A_64
    };

    // initialize device
    while (1)
    {
        esp_err_t r = hx711_init(&dev);
        if (r == ESP_OK){
         printf("Initialized HX711: \n");
            break;
        }
        printf("Could not initialize HX711: %d (%s)\n", r, esp_err_to_name(r));
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }

    // read from device
    while (1)
    {
        esp_err_t r = hx711_wait(&dev, 500);
        if (r != ESP_OK)
        {
            printf("Device not found: %d (%s)\n", r, esp_err_to_name(r));
            continue;
        }

        int32_t data;
        r = hx711_read_data(&dev, &data);
        if (r != ESP_OK)
        {
            printf("Could not read data: %d (%s)\n", r, esp_err_to_name(r));
            continue;
        }

        printf("Raw data: %d\n", data);

        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void app_main()
{
    xTaskCreate(test, "test", configMINIMAL_STACK_SIZE * 5, NULL, 5, NULL);
}