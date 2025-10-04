#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <bb_scd41.h>

SCD41 co2;

extern "C" {
void app_main(void);
}

// I2C Bus on Xiao ESP32C3
#define SDA_PIN 6
#define SCL_PIN 7

void app_main(void)
{
int i;

        printf("bb_scd41 basic example\n");
        printf("Initializes, configures, then loops displaying the sensor values.\n");
        i = co2.init(SDA_PIN, SCL_PIN);
        if (i != SCD41_SUCCESS) {
            printf("CO2 sensor not found!\n");
            while (1) {
                vTaskDelay(1);
            }
        }
        printf("CO2 sensor detected and initialized\n");
        co2.start(); // start with default options
        while (1) {
                co2.getSample();
                printf("CO2: %d\n", co2.co2());
                vTaskDelay(500); // 1 sample every 5 seconds
        }
} /* app_main() */
