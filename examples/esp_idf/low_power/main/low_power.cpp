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

        printf("This demo shows how to configure the SCD41\nfor low power mode\n");
        i = co2.init(SDA_PIN, SCL_PIN); // find a supported proximity sensor
        if (i != SCD41_SUCCESS) {
            printf("No supported device found\n");
            while (1) {
                vTaskDelay(1);
            }
        }

        printf("SCD4x sensor detected!\n");
        printf("Starting in low power mode\none sample every 30 seconds.\n");
        co2.start(SCD41_MODE_LP_PERIODIC); // start the sensor in low power mode
        while (1) {
                co2.getSample();
                printf("CO2: %d\n", co2.co2());
                vTaskDelay(3000); // one sample every 30 seconds
        }

} /* app_main() */
