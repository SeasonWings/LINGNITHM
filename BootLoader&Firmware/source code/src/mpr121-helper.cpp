/*
 	Support class for MPR121, giving full control over the device efficiently.

 	Modifications licensed under University of Illinois/NCSA Open Source License

 	Copyright (c) 2020 Fang Lu
 	All rights reserved.

 	Permission is hereby granted, free of charge, to any person obtaining a copy
 	of this software and associated documentation files (the "Software"), to
 	deal with the Software without restriction, including without limitation the
 	rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 	sell copies of the Software, and to permit persons to whom the Software is
 	furnished to do so, subject to the following conditions:

 	- Redistributions of source code must retain the above copyright notice,
 	  this list of conditions and the following disclaimers.
 	- Redistributions in binary form must reproduce the above copyright notice,
 	  this list of conditions and the following disclaimers in the documentation
 	  and/or other materials provided with the distribution.
 	- Neither the names of Fang Lu, nor the names of its contributors may be
 	  used to endorse or promote products derived from this Software without
 	  specific prior written permission.

 	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 	CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 	WITH THE SOFTWARE.

 	Original license text from Adafruit

    @file Adafruit_MPR121.cpp

    This is a library for the MPR121 12-Channel Capacitive Sensor

    Designed specifically to work with the MPR121 board.

    Pick one up today in the adafruit shop!
    ------> https://www.adafruit.com/product/1982

    These sensors use I2C to communicate, 2+ pins are required to interface

    Adafruit invests time and resources providing this open source code,
    please support Adafruit andopen-source hardware by purchasing products
    from Adafruit!

    Limor Fried/Ladyada (Adafruit Industries).

    BSD license, all text above must be included in any redistribution
*/

#include "mpr121-helper.h"
#include "mpr121-config.h"



void MPR121::begin(uint8_t addr, TwoWire *wire) {
  _i2caddr = addr;
  _wire = wire;
  _wire->begin();
}

void MPR121::init() {
  // 软件重置
  reset();
  delay(1);
  stop();

  writeRegisterRaw(MPR121_MHDR, MPR121CONF_MHDR, false);
  writeRegisterRaw(MPR121_NHDR, MPR121CONF_NHDR, false);
  writeRegisterRaw(MPR121_NCLR, MPR121CONF_NCLR, false);
  writeRegisterRaw(MPR121_FDLR, MPR121CONF_FDLR, false);
  writeRegisterRaw(MPR121_MHDF, MPR121CONF_MHDF, false);
  writeRegisterRaw(MPR121_NHDF, MPR121CONF_NHDF, false);
  writeRegisterRaw(MPR121_NCLF, MPR121CONF_NCLF, false);
  writeRegisterRaw(MPR121_FDLF, MPR121CONF_FDLF, false);
  writeRegisterRaw(MPR121_NHDT, MPR121CONF_NHDT, false);
  writeRegisterRaw(MPR121_NCLT, MPR121CONF_NCLT, false);
  writeRegisterRaw(MPR121_FDLT, MPR121CONF_FDLT, false);
  // Skip ELEPROX baseline controls
  for (int i = 0; i < 12; i++) {//设置触发阈值和充放电流时间
    writeRegisterRaw(MPR121_TOUCHTH(i), MPR121CONF_TTH, false);
    writeRegisterRaw(MPR121_RELEASETH(i), MPR121CONF_RTH, false);
    writeRegisterRaw(MPR121_CDC(i), 0, false);
    writeRegisterRaw(MPR121_CDT(i), 0, false);
  }
  writeRegisterRaw(MPR121_DEBOUNCE, (MPR121CONF_DR << 4) | MPR121CONF_DT, false); //设置采样数
  writeRegisterRaw(MPR121_AFE1, (MPR121CONF_FFI << 6) | MPR121CONF_CDC, false);
  writeRegisterRaw(MPR121_AFE2, (MPR121CONF_CDT << 5) | (MPR121CONF_SFI << 3) | MPR121CONF_ESI, false);
  _ecr = (MPR121CONF_CL << 6) | (MPR121CONF_ELEPROX << 4) | MPR121CONF_ELE; // Write when start
  // Skip GPIO configurations
  writeRegisterRaw(MPR121_AUTOCONF0,
                   (MPR121CONF_AFES << 6) | (MPR121CONF_RETRY << 4) | (MPR121CONF_BVA << 2) | (MPR121CONF_ARE << 1) | MPR121CONF_ACE,
                   false);
  writeRegisterRaw(MPR121_AUTOCONF1,
                   (MPR121CONF_SCTS << 7) | (MPR121CONF_OORIE << 2) | (MPR121CONF_ARFIE << 1) | MPR121CONF_ACFIE,
                   false);
  writeRegisterRaw(MPR121_USL, MPR121CONF_USL, false);
  writeRegisterRaw(MPR121_LSL, MPR121CONF_LSL, false);
  writeRegisterRaw(MPR121_TL, MPR121CONF_TL, false);

  // Manually call run to start
}

void MPR121::writeRegister(uint8_t reg, uint8_t value) {//相同
  // MPR121 must be put in Stop Mode to write to most registers
  bool stop_required = true;
  if (reg == MPR121_ECR || (0x73 <= reg && reg <= 0x7A)) {
    stop_required = false;
  }
  if (stop_required) {
    _wire->beginTransmission(_i2caddr);
    _wire->write(MPR121_ECR);
    _wire->write(0x00); // clear this register to set stop modus
    _wire->endTransmission(false);
  }

  _wire->beginTransmission(_i2caddr);
  _wire->write((uint8_t)reg);
  _wire->write((uint8_t)value);
  _wire->endTransmission(!stop_required);

  if (stop_required) {
    _wire->beginTransmission(_i2caddr);
    _wire->write(MPR121_ECR);
    _wire->write(_ecr); // restore ECR
    _wire->endTransmission(true);
  }
}

//#endif
