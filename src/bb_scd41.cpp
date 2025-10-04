#include <Arduino.h>
#include <BitBang_I2C.h>
#include "bb_scd41.h"

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
void SCD41::getSample()
{
uint8_t ucTemp[16];
uint16_t u16Status;

    if (_iMode == SCD41_MODE_SINGLE_SHOT) {
        sendCMD(SCD41_CMD_SINGLE_SHOT_MEASUREMENT);
        delay(5000); // wait for measurement to occur
    }
    u16Status = readRegister(SCD41_CMD_GET_DATA_READY_STATUS);
//Serial.print("status = 0x"); Serial.println(u16Status, HEX);

    if ((u16Status & 0x07ff) == 0x0000) { // lower 11 bits == 0 -> data not ready
  //     Serial.println("data not ready!");
       return;
    }
    sendCMD(SCD41_CMD_READ_MEASUREMENT);
    delay(5);
    I2CRead(&_bbi2c, _iAddr, ucTemp, 9); // 9 bytes of data for the 3 fields
//Serial.println("Got 9 bytes from sensor");
    _iCO2 = ((uint16_t)ucTemp[0] << 8) | ucTemp[1];
    _iTemperature = (ucTemp[3] << 8) | ucTemp[4];
    _iHumidity = ((uint16_t)ucTemp[6] << 8) | ucTemp[7];    
    _iTemperature = -450 + ((_iTemperature) * 1750L / 65536L);
    _iHumidity = (_iHumidity * 100L) / 65536L;

} /* getSample() */

void SCD41::wakeup()
{
    sendCMD(SCD41_CMD_WAKEUP);
    delay(20);
} /* wakeup() */

int SCD41::stop()
{
    sendCMD(SCD41_CMD_STOP_PERIODIC_MEASUREMENT);
    delay(500); // wait for it to execute
    return SCD41_SUCCESS;
} /* stop() */

int SCD41::start(int iMode)
{
     if (iMode < SCD41_MODE_PERIODIC || iMode > SCD41_MODE_SINGLE_SHOT)
        return SCD41_INVALID_PARAM;
     _iMode = iMode;
// Start correct mode
     wakeup();
     delay(5);
     if (iMode == SCD41_MODE_PERIODIC)
        sendCMD(SCD41_CMD_START_PERIODIC_MEASUREMENT);
     else if (iMode == SCD41_MODE_LP_PERIODIC)
        sendCMD(SCD41_CMD_START_LP_PERIODIC_MEASUREMENT);
     else // single shot is essentially "stopped"
        sendCMD(SCD41_CMD_STOP_PERIODIC_MEASUREMENT);
     delay(1);
     return SCD41_SUCCESS;
} /* start() */

void SCD41::setAutoCalibrate(bool bOn)
{
    sendCMD(SCD41_CMD_SET_AUTOMATIC_SELF_CALIBRATION_ENABLED, bOn);
} /* setAutoCalibrate() */

int SCD41::recalibrate(uint16_t u16CO2)
{
uint8_t ucTemp[4];

        sendCMD(SCD41_CMD_FORCE_RECALIBRATE, u16CO2); // set the reference CO2 level at 423ppm
        delay(400); // wait to complete
    I2CRead(&_bbi2c, _iAddr, ucTemp, 3); // 3 byte response. 0xFFFF = failed
    if (ucTemp[0] == 0xff && ucTemp[1] == 0xff)
        return SCD41_ERROR;
    else
        return SCD41_SUCCESS;
} /* recalibrate() */

int SCD41::init(int iSDA, int iSCL, bool bBitBang, int32_t iSpeed)
{
	_bbi2c.bWire = !bBitBang; // use bit bang?
	_bbi2c.iSDA = iSDA;
	_bbi2c.iSCL = iSCL;
	I2CInit(&_bbi2c, iSpeed);
        _iAddr = 0x62;
        return SCD41_SUCCESS;
} /* init() */

uint16_t SCD41::readRegister(uint16_t u16Register)
{
uint8_t ucTemp[4];
uint16_t u16;

   ucTemp[0] = (uint8_t)(u16Register >> 8);
   ucTemp[1] = (uint8_t)(u16Register);
   I2CWrite(&_bbi2c, _iAddr, ucTemp, 2);
   delay(5);
   I2CRead(&_bbi2c, _iAddr, ucTemp, 3); // ignore CRC for now
   u16 = (uint16_t)ucTemp[0] << 8 | ucTemp[1];
   return u16;

} /* readRegister() */

void SCD41::sendCMD(uint16_t u16Cmd)
{
uint8_t ucTemp[4];

   ucTemp[0] = (uint8_t)(u16Cmd >> 8);
   ucTemp[1] = (uint8_t)(u16Cmd);
   I2CWrite(&_bbi2c, _iAddr, ucTemp, 2);
} /* sendCMD() */

void SCD41::sendCMD(uint16_t u16Cmd, uint16_t u16Parameter)
{
uint8_t ucTemp[8];

   ucTemp[0] = (uint8_t)(u16Cmd >> 8);
   ucTemp[1] = (uint8_t)(u16Cmd);
   ucTemp[2] = (uint8_t)(u16Parameter >> 8);
   ucTemp[3] = (uint8_t)(u16Parameter);
   ucTemp[4] = computeCRC8(&ucTemp[2], 2); // CRC for arguments only
   I2CWrite(&_bbi2c, _iAddr, ucTemp, 5);

} /* sendCMD() */
//Given an array and a number of bytes, this calculate CRC8 for those bytes
//CRC is only calc'd on the data portion (two bytes) of the four bytes being sent
//From: http://www.sunshine2k.de/articles/coding/crc/understanding_crc.html
//Tested with: http://www.sunshine2k.de/coding/javascript/crc/crc_js.html
//x^8+x^5+x^4+1 = 0x31
uint8_t SCD41::computeCRC8(uint8_t data[], uint8_t len)
{
  uint8_t crc = 0xFF; //Init with 0xFF

  for (uint8_t x = 0; x < len; x++)
  {
    crc ^= data[x]; // XOR-in the next input byte

    for (uint8_t i = 0; i < 8; i++)
    {
      if ((crc & 0x80) != 0)
        crc = (uint8_t)((crc << 1) ^ 0x31);
      else
        crc <<= 1;
    }
  }

  return crc; //No output reflection
} /* computeCRC8() */

void SCD41::setUnits(int iUnits)
{
   if (iUnits < SCD41_UNIT_COUNT) {
       _iUnit = iUnits;
   }
}

int SCD41::temperature()
{
    if (_iUnit == SCD41_UNIT_CELCIUS) {
       return _iTemperature;
    } else { // convert to farenheit
       return (((_iTemperature * 9)/5)+320);
    }
} /* temp() */

int SCD41::humidity()
{
    return _iHumidity;
} /* humidity() */

int SCD41::co2()
{
    return _iCO2;
} /* co2() */

void SCD41::shutdown()
{
  sendCMD(SCD41_CMD_POWERDOWN);
  delay(1);
} /* shutdown() */
