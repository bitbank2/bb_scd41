//
// SCD4x CO2 sensor library
//
// Written by Larry Bank - 5/16/2022
// email: bitbank@pobox.com
//
// Copyright 2022 BitBank Software, Inc. All Rights Reserved.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//    http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#ifndef __SCD41__
#define __SCD41__

#include <BitBang_I2C.h>

#define SCD41_SUCCESS 0
#define SCD41_ERROR -1
#define SCD41_INVALID_PARAM -2

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
   SCD41_MODE_PERIODIC=0,
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
    SCD41() {_iUnit = SCD41_UNIT_CELCIUS;}
    ~SCD41() {}
    int init(int iSDA=-1, int iSCL=-1, bool bBitBang=false, int32_t iSpeed=100000L);
    int start(int iMode = SCD41_MODE_PERIODIC);
    int stop();
    void wakeup();
    void getSample(); // trigger + read the latest data
    int recalibrate(uint16_t u16CO2);
    void setAutoCalibrate(bool bOn);
    int temperature(); // temperature (C) as an int 10x (e.g. 25.5 = 255)
    int humidity(); // humidity as an int 10x (e.g. 50.5% = 505)
    int co2(); // CO2 as an int
    void shutdown(); // turn off sensor (if possible)
    uint8_t computeCRC8(uint8_t data[], uint8_t len);
    void sendCMD(uint16_t u16Cmd);
    void sendCMD(uint16_t u16Cmd, uint16_t u16Parameter);
    uint16_t readRegister(uint16_t u16Register);
    void setUnits(int iUnits);

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
