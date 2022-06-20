////基础定义////
#include "mpr121-helper.h"
#include "mpr121-config.h"
#include "serial.h"
#include "HID-Project.h"
#include "FastLED.h"
#define Serial SerialUSB
////MPR定义////
#define CA_ADDR 0x5A//mpr121A
#define CB_ADDR 0x5B//mpr121B
#define CC_ADDR 0x5C//mpr121C

uint16_t LastA = -1, CurrA, LastB = -1, CurrB, LastC = -1, CurrC; //存储触摸的状态
static MPR121 capA, capB, capC; //mpr121定义

////串口定义////
#define CLAMP(val, lo, hi) (val < lo ? lo : (val > hi ? hi : val))
uint8_t checkrelease[32];
static bool auto_scan = false;//串口用标记
////按键定义////
KeyboardKeycode KeyCode[32] = {//键值列表
   KEY_I, KEY_COMMA, KEY_8, KEY_K, KEY_U, KEY_M, KEY_7, KEY_J, KEY_Y, KEY_N,
   KEY_6, KEY_H, KEY_T, KEY_B, KEY_5, KEY_G, KEY_R, KEY_V, KEY_4, KEY_F, KEY_E, KEY_C,  
   KEY_3, KEY_D, KEY_W, KEY_X, KEY_2, KEY_S, KEY_Q, KEY_Z, KEY_1, KEY_A,
   }; 
////灯光定义////
uint16_t LED_BRI = 255; //灯亮度
#define LED_PIN 7 //灯针脚
#define LED_NUM 31 //灯数量
CRGB leds[LED_NUM];

uint32_t MiddleLED=CRGB::Black; //中缝灯色
uint32_t GroundLED=CRGB::Aqua; //触摸区常态灯色
uint32_t PressLED=CRGB::Red; //触摸时灯色

////函数指针////
void (*AirSensor)();
void (*AirLED)();
void (*TouchCheck)();
void (*TouchLEDCheck)();


void TouchSetup() {//触摸初始化
  capA.begin(CA_ADDR);
  capA.init();
  capA.run();

  capB.begin(CB_ADDR);
  capB.init();
  capB.run();

  capC.begin(CC_ADDR);
  capC.init();
  capC.run();

  Wire.setClock(800000);//I2C波特率
}

//直连触摸函数部分//
int CalCheck(int bl, int fd) {//触摸数值计算
  int cal = bl - fd;
  return cal > 4 ? 5 * cal : (cal > 0 ? cal : 0) ;
}

//键盘触摸函数部分//
uint8_t ledindex = 0;
uint8_t TouchState[32];
uint8_t val = 30;
void K32Check() {
  int16_t bl, fl, cal, calpress[32];
  for (uint8_t i = 0; i < 12; i++) {//计算数值
    if (i < 10) {
      bl = capA.baselineData(i);
      fl = capA.filteredData(i);
      cal = CalCheck(bl, fl);
      calpress[i] = CLAMP(cal, 0, 255);
    }
      bl = capB.baselineData(i);
      fl = capB.filteredData(i);
      cal = CalCheck(bl, fl);
      calpress[i + 10] = CLAMP(cal, 0, 255);
    if (i < 10) {
      bl = capC.baselineData(i);
      fl = capC.filteredData(i);
      cal = CalCheck(bl, fl);
      calpress[i + 10 + 12] = CLAMP(cal, 0, 255);
    }
  }
  for (uint8_t i = 0; i < 32; i++) {
    uint8_t calkeypress = calpress[i];
    Serial.println(calpress[i]);
    if (ledindex <= 15) {//初始化完成后的灯
      leds[31 - i * 2] = MiddleLED;
      leds[30 - i * 2] = GroundLED;
      FastLED[0].showLeds(255);
      ledindex++;
    }
    if (calkeypress >= val) {//Check
      checkrelease[i] = SLIDER_CMD_AUTO_SCAN;
      NKROKeyboard.press(KeyCode[i]);
      i%2==0?leds[i] = PressLED:leds[i-1] = PressLED;
    }
    else {
      if (checkrelease[i]) {
        checkrelease[i] = 0;
        NKROKeyboard.release(KeyCode[i]);
        i%2==0?leds[i] = GroundLED:leds[i-1] = GroundLED;
      }
      continue;
    }
  }
  leds[7] = PressLED;
  leds[15] = PressLED;
  leds[23] = PressLED;
  FastLED[0].showLeds(255);
}

