//
// bb_scd41 Arduino example
// This project shows how to display the sensor values
// on an I2C SSD1306 OLED display
// It depends on the OneBitDisplay library (also in the Arduino library manager)
//
#include <bb_scd41.h>
#include <OneBitDisplay.h>
SCD41 co2;
ONE_BIT_DISPLAY obd;

void setup()
{
  co2.init();
  obd.I2Cbegin(OLED_128x64, 0x3c); // start OLED on default I2C bus (known on Xiao boards)
  obd.fillScreen(OBD_WHITE);
  obd.setTextColor(OBD_BLACK);
  obd.setFont(FONT_12x16);
  obd.println("BitBank");
  obd.println("SCD41");
}

void loop()
{
  co2.start(); // start with default options
  obd.setFont(FONT_8x8);
  while (1) {
      char szTemp[32];
      co2.getSample();
      sprintf(szTemp, "CO2: %d    ", co2.co2());
      obd.drawString(szTemp, 0, 40);
      sprintf(szTemp, "Temp: %d.%dC  ", co2.temperature()/10, co2.temperature() % 10);
      obd.drawString(szTemp, 0, 48);
      sprintf(szTemp, "Humidity: %d%% ", co2.humidity());
      obd.drawString(szTemp, 0, 56);
      delay(5000); // show a new sample every 5 seconds
  }
}

