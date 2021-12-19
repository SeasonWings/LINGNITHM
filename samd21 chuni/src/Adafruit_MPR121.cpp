/*!
 * @file Adafruit_MPR121.cpp
 *
 *  @mainpage Adafruit MPR121 arduino driver
 *
 *  @section intro_sec Introduction
 *
 *  This is a library for the MPR121 I2C 12-chan Capacitive Sensor
 *
 *  Designed specifically to work with the MPR121 sensor from Adafruit
 *  ----> https://www.adafruit.com/products/1982
 *
 *  These sensors use I2C to communicate, 2+ pins are required to
 *  interface
 *
 *  Adafruit invests time and resources providing this open source code,
 *  please support Adafruit and open-source hardware by purchasing
 *  products from Adafruit!
 *
 *  @section author Author
 *
 *  Written by Limor Fried/Ladyada for Adafruit Industries.
 *
 *  @section license License
 *
 *  BSD license, all text here must be included in any redistribution.
 */

#include "Adafruit_MPR121.h"
Adafruit_MPR121::Adafruit_MPR121() {}
boolean Adafruit_MPR121::begin(uint8_t i2caddr, TwoWire *theWire,
                               uint8_t touchThreshold,
                               uint8_t releaseThreshold) {
  _i2caddr = i2caddr;
  _wire = theWire;
  _wire->begin();
  writeRegister(MPR121_SOFTRESET, 0x63);
  delay(1);
  writeRegister(MPR121_ECR, 0x0);
  uint8_t c = readRegister8(MPR121_CONFIG2);
  if (c != 0x24)
    return false;
  setThresholds(touchThreshold, releaseThreshold);
  writeRegister(MPR121_MHDR, 0x01);
  writeRegister(MPR121_NHDR, 0x01);
  writeRegister(MPR121_NCLR, 0x01);
  writeRegister(MPR121_FDLR, 0x00);
  writeRegister(MPR121_MHDF, 0x01);
  writeRegister(MPR121_NHDF, 0x01);
  writeRegister(MPR121_NCLF, 0x01);
  writeRegister(MPR121_FDLF, 0x00);
  writeRegister(MPR121_NHDT, 0x00);
  writeRegister(MPR121_NCLT, 0x00);
  writeRegister(MPR121_FDLT, 0x00);
  writeRegister(MPR121_DEBOUNCE, 0x01);
  writeRegister(MPR121_CONFIG1, 0xF0);
  writeRegister(MPR121_CONFIG2, 0x40);
#ifdef AUTOCONFIG
  writeRegister(MPR121_AUTOCONFIG0, 0x0B);
  writeRegister(MPR121_UPLIMIT, 200);
  writeRegister(MPR121_TARGETLIMIT, 180);
  writeRegister(MPR121_LOWLIMIT, 130);
#endif
  byte ECR_SETTING =
      B10000000 + 12;
  writeRegister(MPR121_ECR, ECR_SETTING);
  return true;
}
void Adafruit_MPR121::setThreshholds(uint8_t touch, uint8_t release) {
  setThresholds(touch, release);
}
void Adafruit_MPR121::setThresholds(uint8_t touch, uint8_t release) {
  writeRegister(MPR121_ECR, 0x00);
  for (uint8_t i = 0; i < 12; i++) {
    writeRegister(MPR121_TOUCHTH_0 + 2 * i, touch);
    writeRegister(MPR121_RELEASETH_0 + 2 * i, release);
  }
  writeRegister(MPR121_ECR, 0x8F);
}
uint16_t Adafruit_MPR121::filteredData(uint8_t t) {
  if (t > 12)
    return 0;
  return readRegister16(MPR121_FILTDATA_0L + t * 2);
}
uint16_t Adafruit_MPR121::baselineData(uint8_t t) {
  if (t > 12)
    return 0;
  uint16_t bl = readRegister8(MPR121_BASELINE_0 + t);
  return (bl << 2);
}
uint16_t Adafruit_MPR121::touched(void) {
  uint16_t t = readRegister16(MPR121_TOUCHSTATUS_L);
  return t & 0x0FFF;
}
uint8_t Adafruit_MPR121::readRegister8(uint8_t reg) {
  _wire->beginTransmission(_i2caddr);
  _wire->write(reg);
  _wire->endTransmission(false);
  _wire->requestFrom(_i2caddr, 1);
  if (_wire->available() < 1)
    return 0;
  return (_wire->read());
}
uint16_t Adafruit_MPR121::readRegister16(uint8_t reg) {
  _wire->beginTransmission(_i2caddr);
  _wire->write(reg);
  _wire->endTransmission(false);
  _wire->requestFrom(_i2caddr, 2);
  if (_wire->available() < 2)
    return 0;
  uint16_t v = _wire->read();
  v |= ((uint16_t)_wire->read()) << 8;
  return v;
}
void Adafruit_MPR121::writeRegister(uint8_t reg, uint8_t value) {
  bool stop_required = true;
  uint8_t ECR = readRegister8(
      MPR121_ECR);
  if (reg == MPR121_ECR || (0x73 <= reg && reg <= 0x7A)) {
    stop_required = false;
  }
  if (stop_required) {
    _wire->beginTransmission(_i2caddr);
    _wire->write(MPR121_ECR);
    _wire->write((byte)0x00);
    _wire->endTransmission();
  }

  _wire->beginTransmission(_i2caddr);
  _wire->write((uint8_t)reg);
  _wire->write((uint8_t)(value));
  _wire->endTransmission();

  if (stop_required) {
    _wire->beginTransmission(_i2caddr);
    _wire->write(MPR121_ECR);
    _wire->write(ECR);
    _wire->endTransmission();
  }
}
