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

    @file Adafruit_MPR121.h

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

#ifndef _MPR121_HELPER_H
#define _MPR121_HELPER_H

#include "Arduino.h"
#include <Wire.h>

// Configuration values

#define MPR121_FFI_6	0
#define MPR121_FFI_10	1
#define MPR121_FFI_18	2
#define MPR121_FFI_34	3

#define MPR121_CDT_INHERIT	0
#define MPR121_CDT_500NS	1
#define MPR121_CDT_1US		2
#define MPR121_CDT_2US		3
#define MPR121_CDT_4US		4
#define MPR121_CDT_8US		5
#define MPR121_CDT_16US		6
#define MPR121_CDT_32US		7

#define MPR121_SFI_4	0
#define MPR121_SFI_6	0
#define MPR121_SFI_10	0
#define MPR121_SFI_18	0

#define MPR121_ESI_1MS		0
#define MPR121_ESI_2MS		1
#define MPR121_ESI_4MS		2
#define MPR121_ESI_8MS		3
#define MPR121_ESI_16MS		4
#define MPR121_ESI_32MS		5
#define MPR121_ESI_64MS		6
#define MPR121_ESI_128MS	7

#define MPR121_CL_NONE	0
#define MPR121_CL_LOCK	1
#define MPR121_CL_INIT1	2
#define MPR121_CL_INTI2	3

#define MPR121_ELEPROX_0	0
#define MPR121_ELEPROX_2	1
#define MPR121_ELEPROX_4	2
#define MPR121_ELEPROX_12	3

#define MPR121_AFES_6	0
#define MPR121_AFES_10	1
#define MPR121_AFES_18	2
#define MPR121_AFES_34	3

#define MPR121_RETRY_0	0
#define MPR121_RETRY_2	1
#define MPR121_RETRY_4	2
#define MPR121_RETRY_8	3

// Device register map

#define MPR121_TOUCHSTATUS_L 0x00
#define MPR121_TOUCHSTATUS_H 0x01
#define MPR121_OOR_L 0x02
#define MPR121_OOR_R 0x03
#define MPR121_FILTDATA(ch) (0x04 + 2*(ch))
#define MPR121_BASELINE(ch) (0x1E + (ch))
#define MPR121_MHDR 0x2B
#define MPR121_NHDR 0x2C
#define MPR121_NCLR 0x2D
#define MPR121_FDLR 0x2E
#define MPR121_MHDF 0x2F
#define MPR121_NHDF 0x30
#define MPR121_NCLF 0x31
#define MPR121_FDLF 0x32
#define MPR121_NHDT 0x33
#define MPR121_NCLT 0x34
#define MPR121_FDLT 0x35
#define MPR121_MHDPROXR 0x36
#define MPR121_HHDPROXR 0x37
#define MPR121_NCLPROXR 0x38
#define MPR121_FDLPROXR 0x39
#define MPR121_MHDPROXF 0x3A
#define MPR121_NHDPROXF 0x3B
#define MPR121_NCLPROXF 0x3C
#define MPR121_FDLPROXF 0x3D
#define MPR121_NHDPROXT 0x3E
#define MPR121_NCLPROXT 0x3F
#define MPR121_FDLPROXT 0x40
#define MPR121_TOUCHTH(ch) (0x41 + 2*(ch))
#define MPR121_RELEASETH(ch) (0x42 + 2*(ch))
#define MPR121_DEBOUNCE 0x5B
#define MPR121_AFE1 0x5C
#define MPR121_AFE2 0x5D
#define MPR121_ECR 0x5E
#define MPR121_CDC(ch) (0x5F + (ch))
#define MPR121_CDT(ch) (0x6C + ((ch)>>1))
#define MPR121_CTL0 0x73
#define MPR121_CTL1 0x74
#define MPR121_DAT 0x75
#define MPR121_DIR 0x76
#define MPR121_EN 0x77
#define MPR121_SET 0x78
#define MPR121_CLR 0x79
#define MPR121_TOG 0x7A
#define MPR121_AUTOCONF0 0x7B
#define MPR121_AUTOCONF1 0x7C
#define MPR121_USL 0x7D
#define MPR121_LSL 0x7E
#define MPR121_TL 0x7F
#define MPR121_SRST 0x80

#define MPR121_I2CADDR_DEFAULT 0x5A

class MPR121 {
  public:
    MPR121() {}

    void begin(uint8_t addr = MPR121_I2CADDR_DEFAULT, TwoWire *wire = &Wire);
    void init();

    uint8_t readRegister8(uint8_t rs) {//相同
      _wire->beginTransmission(_i2caddr);
      _wire->write(rs);
      _wire->endTransmission(false);
      _wire->requestFrom(_i2caddr, 1);
      if (_wire->available() < 1)
        return 0;
      return _wire->read();
    }
    uint16_t readRegister16(uint8_t rs) {//相同
      _wire->beginTransmission(_i2caddr);
      _wire->write(rs);
      _wire->endTransmission(false);
      _wire->requestFrom(_i2caddr, 2);
      if (_wire->available() < 2)
        return 0;
      uint16_t v = _wire->read();
      v |= ((uint16_t)_wire->read()) << 8;
      return v;
    }
    void writeRegister(uint8_t rd, uint8_t value);
    void writeRegisterRaw(uint8_t rd, uint8_t value, bool stop = true) {
      _wire->beginTransmission(_i2caddr);
      _wire->write(rd);
      _wire->write(value);
      _wire->endTransmission(stop);
    }

    void stop() {
      writeRegisterRaw(MPR121_ECR, 0x00);
    }
    void run() {
      writeRegisterRaw(MPR121_ECR, _ecr);
    }
    void reset() {
      writeRegisterRaw(MPR121_SRST, 0x63);
    }

    uint16_t filteredData(uint8_t ch) {
      return readRegister16(MPR121_FILTDATA(ch));
    }
    uint16_t baselineData(uint8_t ch) {
      return ((uint16_t)readRegister8(MPR121_BASELINE(ch))) << 2;
    }
    uint16_t touched() {
      return readRegister16(MPR121_TOUCHSTATUS_L);
    }

  private:
    int8_t _i2caddr;
    TwoWire *_wire;
    uint8_t _ecr;
};

#endif
