//
// bb_scd41 "shared bus" example
// This example shows how to find the CO2 sensor, no
// matter which I2C bus it's attached to, and share that same I2C bus
// with another BitBank library (OneBitDisplay)
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <dirent.h>
#include <bb_scd41.h>
#include <OneBitDisplay.h>

ONE_BIT_DISPLAY obd;
SCD41 co2;

int main(int argc, char *argv[])
{
int i;

        printf("bb_scd41 shared bus example\n");
        printf("Initializes, configures, and loops displaying the sensor values.\n");
	printf("The default power mode is continuous sampling every 5 seconds.\n");
	i = co2.init(4); // I2C bus number
	if (i == SCD41_SUCCESS) {
            printf("Sensor found!\n");
	} else {
	    printf("Sensor not found; check your connections\n");
	    return -1;
	}
	// Pass the I2C handle to OneBitDisplay to share for the OLED display
	obd.setBB(co2.getBB());
	obd.I2Cbegin(OLED_128x64, 0x3c);
	obd.fillScreen(OBD_WHITE); // color is relative to B/W LCDs
	obd.setFont(FONT_12x16);
	obd.drawString("BitBank", 0, 0);
	obd.drawString("SCD41", 0, 16);
        obd.setFont(FONT_8x8);
	co2.start(); // start reading CO2 values
	while (1) {
		char szTemp[64];
		co2.getSample();
		sprintf(szTemp, "CO2: %d    ", co2.co2());
		obd.drawString(szTemp, 0, 40);
		sprintf(szTemp, "Temp: %d.%dC  ", co2.temperature()/10, co2.temperature() % 10);
		obd.drawString(szTemp, 0, 48);
		sprintf(szTemp, "Humidity: %d%% ", co2.humidity());
		obd.drawString(szTemp, 0, 56);
		usleep(5000000); // show a new sample every 5 seconds
	}
return 0;
} /* main() */
