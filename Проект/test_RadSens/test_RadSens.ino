#include <Arduino.h>
#include <Wire.h>
#include "CG_RadSens.h"

#define I2C_SDA 33
#define I2C_SCL 32

CG_RadSens radSens(RS_DEFAULT_I2C_ADDRESS); // 0x66

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println(F("Start RadSens test..."));
  
  delay(1000);
  
  while(!radSens.init()) {
    Serial.println("RadSens connectind error");
    delay(1000);
  }

  set();
  info();

  delay(3000);
} // setup

void set() {
  radSens.setSensitivity(105);
}

void info() {
  uint8_t sensorChipId  = radSens.getChipId(); // default 0x7D
  uint8_t firmWareVer   = radSens.getFirmwareVersion();
  uint16_t sensitivity  = radSens.getSensitivity();
  bool hvGeneratorState = radSens.getHVGeneratorState();
  bool ledState         = radSens.getLedState();
  
  Serial.print("Chip id:     0x"); Serial.println(sensorChipId, HEX);
  Serial.print("Firmware:    ");   Serial.println(firmWareVer);
  Serial.print("Sensitivity: ");   Serial.println(sensitivity);
  Serial.print("Generator:   ");   Serial.println(hvGeneratorState);
  Serial.print("LED state:   ");   Serial.println(ledState);
  Serial.println();
}

void generatorOn()  {radSens.setHVGeneratorState(true);}
void generatorOff() {radSens.setHVGeneratorState(false);}

void ledOn()  {radSens.setLedState(true);}
void ledOff() {radSens.setLedState(false);}

void loop() {
  Serial.print("\tstt:"); Serial.print(radSens.getRadIntensyStatic());  // static
  Serial.print("\tdyn:"); Serial.print(radSens.getRadIntensyDynamic()); // dynamic
  Serial.print("\tpls:"); Serial.print(radSens.getNumberOfPulses());    // number of pulses
  Serial.println();
  delay(2000);
}