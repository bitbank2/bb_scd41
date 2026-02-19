//
// BitBank Sensirion SCD40/41 CO2 Sensor Library
// Written by Larry Bank
//
// SPDX-FileCopyrightText: 2022 Larry Bank <bitbank@pobox.com>
// SPDX-License-Identifier: Apache-2.0
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//    http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//===========================================================================
//
#ifndef __SCD41__
#define __SCD41__

#ifdef __LINUX__
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
#include <time.h>

#else // !LINUX

#ifdef ARDUINO
#include <Arduino.h>
#ifndef __AVR_ATtiny85__
#include <Wire.h>
#endif // !AVR
#include <BitBang_I2C.h>
#else // ESP_IDF?
#include <stdint.h>
#endif // ARDUINO
#endif // !__LINUX__

// For Linux and esp-idf we add a file/device handle member
// to the BBI2C structure
#if !defined( ARDUINO ) && !defined(__BB_I2C__)
#define __BB_I2C__
typedef struct _tagbbi2c
{
  int file_i2c;
  uint8_t iSDA, iSCL;
  uint8_t bWire;
} BBI2C;
#endif

#define SCD41_SUCCESS 0
#define SCD41_ERROR -1
#define SCD41_INVALID_PARAM -2
#define SCD41_NOT_READY -3

// 16-bit I2C commands
#define SCD41_CMD_START_PERIODIC_MEASUREMENT              0x21b1
#define SCD41_CMD_START_LP_PERIODIC_MEASUREMENT           0x21ac
#define SCD41_CMD_SINGLE_SHOT_MEASUREMENT                 0x219d
#define SCD41_CMD_READ_MEASUREMENT                        0xec05 // execution time: 1ms
#define SCD41_CMD_STOP_PERIODIC_MEASUREMENT               0x3f86 // execution time: 500ms
#define SCD41_CMD_SET_AUTOMATIC_SELF_CALIBRATION_ENABLED  0x2416 // execution time 1ms
#define SCD41_CMD_GET_DATA_READY_STATUS                   0xe4b8 // execution time: 1ms
#define SCD41_CMD_POWERDOWN                               0x36e0 // execution time: 1ms
#define SCD41_CMD_WAKEUP                                  0x36f6 // execution time: 20ms
#define SCD41_CMD_FORCE_RECALIBRATE                       0x362f // execution time: 400ms


enum {
   SCD41_MODE_OFF=0,
   SCD41_MODE_IDLE,
   SCD41_MODE_PERIODIC,
   SCD41_MODE_LP_PERIODIC,
   SCD41_MODE_SINGLE_SHOT
};

enum {
   SCD41_UNIT_CELCIUS = 0,
   SCD41_UNIT_FARENHEIT,
   SCD41_UNIT_COUNT
};

class SCD41
{
  public:
    SCD41() {_iUnit = SCD41_UNIT_CELCIUS; _iMode = SCD41_MODE_OFF;}
    ~SCD41() {}
    int init(int iSDA=-1, int iSCL=-1, bool bBitBang=false, int32_t iSpeed=100000L);
    int init(BBI2C *pBB);
    BBI2C *getBB();
    int start(int iMode = SCD41_MODE_PERIODIC);
    int stop();
    int getMode();
    void wakeup();
    int triggerSample(); // trigger a sample to start (single shot mode only)
    int getSample(); // read the latest sample data
    int recalibrate(uint16_t u16CO2);
    void setAutoCalibrate(bool bOn);
    int temperature(); // temperature (C or F) as an int 10x (e.g. 25.5 = 255)
    int humidity(); // humidity as an int 10x (e.g. 50.5% = 505)
    int co2(); // CO2 as an int
    void shutdown(); // turn off sensor (if possible)
    uint8_t computeCRC8(uint8_t data[], uint8_t len);
    void sendCMD(uint16_t u16Cmd);
    void sendCMD(uint16_t u16Cmd, uint16_t u16Parameter);
    uint16_t readRegister(uint16_t u16Register);
    int setUnits(int iUnits);

  private:
    uint32_t _iCO2, _iHumidity;
    int32_t _iTemperature;
    int _iUnit;
    int _iType; // sensor type (SCD40/SCD41)
    int _iAddr; // I2C address of device
    int _iMode;
    BBI2C _bbi2c;
};

#endif // __SCD41__
