//
// bb_scd41 functional tests
//
#include <bb_scd41.h>
SCD41 co2;

//
// Simple logging print
//
void LOGPRINT(int line, char *string, const char *result)
{
    Serial.printf("Line: %d: msg: %s%s\n", line, string, result);
} /* LOGPRINT() */

void setup()
{
  int rc, iCO2, iSample, iTotal, iTotalPass, iTotalFail;
  const char *szStart = " - START";
  const char *szPassed = " - PASSED";
  const char *szFailed = " - FAILED";
  char *szTestName;

  Serial.begin(115200);
  delay(3000); // allow time for CDC-USB to start
  Serial.println("SCD41 function tests");
  iTotal = iTotalPass = iTotalFail = 0;

// Test 1 - initialize the sensor
  szTestName = (char *)"SCD4x init test";
  iTotal++;
  LOGPRINT(__LINE__, szTestName, szStart);
  rc = co2.init(); // use default I2C pin definition
  if (rc == SCD41_SUCCESS) {
      LOGPRINT(__LINE__, szTestName, szPassed);
      iTotalPass++;
  } else {
      LOGPRINT(__LINE__, szTestName, szFailed);
      iTotalFail++;
  }
// Test 2 - set the sensor to normal power mode
  szTestName = (char *)"SCD4x periodic mode test";
  iTotal++;
  LOGPRINT(__LINE__, szTestName, szStart);
  rc = co2.start(SCD41_MODE_PERIODIC); // start with default options
  if (rc == SCD41_SUCCESS) {
    long lStart, lEnd;
    co2.getSample();
    iSample = co2.co2(); // read a sample now to see if it changes in around 5 seconds
    lStart = millis();
    lEnd = lStart + 5500; // ending time (longer than this is a problem)
    while (co2.co2() == iSample && millis() < lEnd) {
      co2.getSample(); // wait for a change in the sample value
    }
    if (millis() < lEnd) {
      LOGPRINT(__LINE__, szTestName, szPassed);
      iTotalPass++;
    } else {
      Serial.printf("time = %d, co2 = %d\n", (int)(millis() - lStart), co2.co2());
      LOGPRINT(__LINE__, szTestName, szFailed);
      iTotalFail++;
    }
  } else { // initial failure
      LOGPRINT(__LINE__, szTestName, szFailed);
      iTotalFail++;
  }
// Test 3 - stop the sensor
  szTestName = (char *)"SCD4x stop mode test";
  iTotal++;
  LOGPRINT(__LINE__, szTestName, szStart);
  rc = co2.stop();
  if (rc == SCD41_SUCCESS) {
    long lStart, lEnd;
    rc = co2.getSample();
    if (rc == SCD41_ERROR) {
      LOGPRINT(__LINE__, szTestName, szPassed);
      iTotalPass++;
    } else {
      LOGPRINT(__LINE__, szTestName, szFailed);
      iTotalFail++;
    }
  } else {
      LOGPRINT(__LINE__, szTestName, szFailed);
      iTotalFail++;
  }
// Test 4 - set the sensor to low power mode
  szTestName = (char *)"SCD4x low power mode test";
  iTotal++;
  LOGPRINT(__LINE__, szTestName, szStart);
  rc = co2.start(SCD41_MODE_LP_PERIODIC);
  if (rc == SCD41_SUCCESS) {
    long l, lStart, lEnd;
    delay(5000); // the first sample comes in < 5 seconds; wait for the second
    co2.getSample();
    iSample = co2.co2(); // read a sample now to see if it changes in around 5 seconds
    lStart = millis();
    lEnd = lStart + 31000; // ending time (longer than this is a problem)
    while (co2.co2() == iSample && millis() < lEnd) {
      co2.getSample(); // wait for a change in the sample value
    }
    l = millis();
    if (l < lEnd && l > lStart + 7000) { // should be 30 seconds
      LOGPRINT(__LINE__, szTestName, szPassed);
      iTotalPass++;
    } else {
      LOGPRINT(__LINE__, szTestName, szFailed);
      iTotalFail++;
    }
  } else { // initial failure
      LOGPRINT(__LINE__, szTestName, szFailed);
      iTotalFail++;
  }
// Test 5 - unit conversion test
  szTestName = (char *)"SCD4x temperature units test";
  iTotal++;
  LOGPRINT(__LINE__, szTestName, szStart);
  rc = co2.setUnits(SCD41_UNIT_CELCIUS);
  if (rc == SCD41_SUCCESS) {
    int iTC, iTF;
    iTC = co2.temperature();
    co2.setUnits(SCD41_UNIT_FARENHEIT);
    iTF = co2.temperature();
    if ((((iTC * 9)/5) + 320) == iTF) {
      LOGPRINT(__LINE__, szTestName, szPassed);
      iTotalPass++;
    } else {
      LOGPRINT(__LINE__, szTestName, szFailed);
      iTotalFail++;
    }
  } else {
      LOGPRINT(__LINE__, szTestName, szFailed);
      iTotalFail++;
  }
  Serial.printf("Total tests: %d, %d passed, %d failed\n", iTotal, iTotalPass, iTotalFail);
} /* setup() */

void loop()
{

}