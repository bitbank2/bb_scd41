//
// SCD41 CO2 Example Project
//
// Written by Larry Bank
// Copyright (c) 2022 BitBank Software, Inc.
// email: larry@bitbanksoftware.com
//
// Creative Commons license:
// You are free to:
// Share — copy and redistribute the material in any medium or format
// Adapt — remix, transform, and build upon the material for any purpose, even commercially.
//
#include <bb_scd41.h>
SCD41 mySensor;

// Define both of these to be -1 for default I2C pins
// This demo was run on an MCU which has them connected to GPIO 5 and 6
#define SDA_PIN 5
#define SCL_PIN 6
#define BITBANG false

// Converts the CO2 concentration into a quality assessment
char *szAirQ[] = {(char *)"Good", (char *)"So-So", (char *)"Bad", (char *)"Danger", (char *)"Get out!"};

void setup() {
  Serial.begin(115200);
  delay(2000); // wait for USB->Serial to start
  Serial.println("Starting...");
  if (mySensor.init(SDA_PIN, SCL_PIN, BITBANG, 100000) == SCD41_SUCCESS)
  {
    Serial.println("Found SCD41 sensor!");
    mySensor.start(); // start sampling mode
  } else { // can't find the sensor, stop
    Serial.println("SCD41 sensor not found");
    Serial.println("Check your connections");
    Serial.println("\nstopping...");
    while (1) {};
  } // no sensor connected or some error
} /* setup() */

void loop() {
char szTemp[64];
int i, iCO2;

    delay(5000); // 5 seconds per reading
    // The SCD41 takes 5 seconds to return a sample in this mode
    mySensor.getSample();
    iCO2 = mySensor.co2();
    // Convert CO2 value to quality assessment
    Serial.print("AirQ: ");
    if (iCO2 < 440 || iCO2 > 6000)
       Serial.println("N/A");
    else
       Serial.println(szAirQ[iCO2/1000]);
    sprintf(szTemp, "CO2 %d ppm ", iCO2);
    Serial.println(szTemp);
    sprintf(szTemp, "Temperature %dC ", mySensor.temperature());
    Serial.println(szTemp);
    sprintf(szTemp, "Humidity %d%%", mySensor.humidity());
    Serial.println(szTemp);
} /* loop() */
