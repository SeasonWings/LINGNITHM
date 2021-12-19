////基础定义////
#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif
#include "mpr121-helper.h"
#include "mpr121-config.h"
#include "HID-Project.h"
#include "FastLED.h"
#include <Arduino.h>
#define Serial SerialUSB

////MPR定义////
#define CA_ADDR 0x5A//mpr121A
#define CB_ADDR 0x5B//mpr121B
#define CC_ADDR 0x5C//mpr121C

uint16_t LastA = -1, CurrA, LastB = -1, CurrB, LastC = -1, CurrC; //存储触摸的状态
static MPR121 capA, capB, capC; //mpr121

////Air定义////
int calibrationCounter;
bool calibrated;
uint16_t thresholds[6];
uint16_t maxReadings[6];

#define AIR_LED_DELAY 96//轮询延迟（推荐96，128，192，256这几个数值）
#define AIR_INPUT_SENSITIVITY 0.3//红外敏感度（推荐0.1-0.3之间）
#define CALIBRATION_SAMPLES 256//（校准采样数）
#define SKIP_SAMPLES 256//（校准跳过采样数）


int ir_INPUT_pins[6] = {3,4,5,2,6,11};//定义轮询发射红外针脚
int ir_sensor_pins[6] = {A0,A1,A2,A3,A4,A5};//定义轮询接收红外针脚


////按键定义////
KeyboardKeycode KeyCode[32] = {//键值列表
   KEY_3, KEY_D, KEY_W, KEY_X, KEY_2, KEY_S, KEY_Q, KEY_Z, KEY_1, KEY_A,
   KEY_6, KEY_H, KEY_T, KEY_B, KEY_5, KEY_G, KEY_R, KEY_V, KEY_4, KEY_F, KEY_E, KEY_C,  
   KEY_I, KEY_COMMA, KEY_8, KEY_K, KEY_U, KEY_M, KEY_7, KEY_J, KEY_Y, KEY_N, 
   };
   
KeyboardKeycode AirKeys[6] = {KEY_SLASH, KEY_PERIOD, KEY_QUOTE, KEY_SEMICOLON, KEY_RIGHT_BRACE, KEY_LEFT_BRACE};

////串口定义////
#define CLAMP(val, lo, hi) (val < lo ? lo : (val > hi ? hi : val))
static bool auto_scan = false;//串口用标记


////灯光定义////

#define LED_NUM 31 //灯数量
#define LED_PIN 7 //灯针脚
uint16_t LED_BRI= 0xFF; //灯亮度
#define Aqua CRGB::Cyan
#define Red CRGB::Red
CRGB leds[LED_NUM];
//Air灯光定义
#define air_right_pin 10
#define air_right_num 6
CRGB AIR_RIGHT[air_right_num];

#define air_left_pin 8
#define air_left_num 6
CRGB AIR_LEFT[air_left_num];

////函数指针////
void (*MainCheck)();
void (*AirSensor)();

////灯效函数////

void LEDset() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, LED_NUM);
  FastLED.addLeds<WS2812B, air_right_pin, GRB>(AIR_RIGHT, air_right_num);
  FastLED.addLeds<WS2812B, air_left_pin, GRB>(AIR_LEFT, air_left_num);
  
}

uint8_t air_rgb_loop = 0;
uint16_t air_colour = 0;
uint16_t air_colour_state = 0;
void air_rgb_Serial() {
  AIR_RIGHT[air_rgb_loop] = CHSV(air_colour_state, 255, 255);
  AIR_LEFT[air_rgb_loop] = CHSV(air_colour_state, 255, 255);
  air_rgb_loop++;
  if (air_rgb_loop >= 6) {
    air_colour_state++;
    air_rgb_loop = 0;
  }
  if (air_colour_state > 255) {
    air_colour_state = 0;
  }
  for (uint8_t i = 0; i < 6; i++) {
    if (ir_sensor_pins[i]) {
      AIR_RIGHT[i] = 0xffffff;
      AIR_LEFT[i] = 0xffffff;
    }
  }
  FastLED[1].showLeds(255);
  FastLED[2].showLeds(255);
}

//////////Air//////////

void AirSetup() {
  calibrated = false;
  for (int i = 0; i < 6; i++) {
    maxReadings[i] = 0;
  }
  for (int i = 0; i < 6; i++) {
    pinMode(ir_sensor_pins[i], INPUT_PULLUP);
    pinMode(ir_INPUT_pins[i], OUTPUT);
  }
}

void turnOffLight() {//关闭灯光函数
  for(int i = 0;i < 6;i++){
    digitalWrite(ir_INPUT_pins[i], LOW);
    }
}

void changeLight(int light) {//切换红外发射轮询
  switch (light) {
    case 0:
      digitalWrite(ir_INPUT_pins[light], HIGH);
      //Serial.print("一号发射");
      //Serial.print("\t");
      break;
    case 1:
      digitalWrite(ir_INPUT_pins[light], HIGH);
      //Serial.print("二号发射");
      //Serial.print("\t");
      break;
    case 2:
      digitalWrite(ir_INPUT_pins[light], HIGH);
      //Serial.print("三号发射");
      //Serial.print("\t");
      break;
    case 3:
      digitalWrite(ir_INPUT_pins[light], HIGH);
      //Serial.print("四号发射");
      //Serial.print("\t");
      break;
    case 4:
      digitalWrite(ir_INPUT_pins[light], HIGH);
      //Serial.print("五号发射");
      //Serial.print("\t");
      break;
    case 5:
      digitalWrite(ir_INPUT_pins[light], HIGH);
      //Serial.print("六号发射");
      //Serial.print("\t");
      break;
    default:
      turnOffLight();
      break;
  }
}



uint16_t getValue(int sensor) {//获取开灯函数
  changeLight(sensor);// 开灯
  delayMicroseconds(AIR_LED_DELAY);// 加上延迟保证完全开启
  return analogRead(ir_sensor_pins[sensor]);//读数值
}

bool getSensorState(int sensor) {
  turnOffLight();//关灯
  delayMicroseconds(AIR_LED_DELAY);//加上延迟保证完全关闭
  int enviroment = analogRead(ir_sensor_pins[sensor]);//读取环境数值
  int value = getValue(sensor);//读取开灯数值
  
  Serial.print(sensor);
  Serial.print("\t");
  Serial.print(value);
  Serial.print("\t");
  Serial.print(enviroment);
  Serial.print("\t");
  Serial.print(value - enviroment);
  Serial.print("\t");
  Serial.print(value - enviroment + 200);
  Serial.print("\t");
  Serial.print(maxReadings[sensor]);
  Serial.print("\t");
  Serial.print(thresholds[sensor]);
  Serial.println();
  
  //把读数减去环境数值和校准数值比较后返回布尔值，+200为防止负数的比较（这里我不知道咋搞可以防止负数，就这样了）
  return (value - enviroment + 200) < (thresholds[sensor] + 200);
  
}

void AirCalibrate() {
  // 跳过开头采样
  for (int i = 0; i < SKIP_SAMPLES; i++) {
    for (int sensor = 0; sensor < 6; sensor++) {
      getValue(sensor);
      turnOffLight();
    }
  }
  //开始校准
  for (int i = 0; i < CALIBRATION_SAMPLES; i++) {
    for (int sensor = 0; sensor < 6; sensor++) {
        turnOffLight();//关灯
        delayMicroseconds(AIR_LED_DELAY);//加上延迟保证完全关闭
        int enviroment = analogRead(ir_sensor_pins[sensor]);//读取环境数值
        int value = getValue(sensor) - enviroment;//读取开灯数值
      if (value > maxReadings[sensor])//记录最高读数
        maxReadings[sensor] = value;
    }
    Serial.println();
  }
  for (int i = 0; i < 6; i++) {//通过敏感度生成阈值
    thresholds[i] = (AIR_INPUT_SENSITIVITY * maxReadings[i]);
  }
  calibrated = true;//完成校准
}

void AirCheck(){
  if (calibrated){
    for (int i = 0; i < 6; i++){
      if (getSensorState(i)){
        NKROKeyboard.press(AirKeys[i]);
      } else if (!getSensorState(i)){
        NKROKeyboard.release(AirKeys[i]);
      }
    }
  }
}
void NoAir(){}//关闭Air

//////////触摸//////////
void MprCheck() {////触摸扫描（按键）
  bool change = false;
    CurrA = capA.touched();
    if (CurrA != LastA) {
      for (uint8_t i = 0; i < 10; i++) {
        if ((CurrA & _BV(i))) {
          NKROKeyboard.press(KeyCode[i + 22]); 
          if(i%2==0){
            leds[i] = Red;}
          else if(i%2==1){
            leds[i-1] = Red;}
        } else if (!(CurrA & _BV(i))) {
          NKROKeyboard.release(KeyCode[i + 22]);
          if(i%2==0){
            leds[i] = Aqua;}
          else if(i%2==1){
            leds[i] = Red;}
        }
      }
      change = true;
      LastA = CurrA;
    }
    CurrB = capB.touched();
    if (CurrB != LastB) {
      for (uint8_t i = 0; i < 12; i++) {
        if ((CurrB & _BV(i))) {
          NKROKeyboard.press(KeyCode[i + 10]);
          if(i%2==0){
            leds[i + 10] = Red;}
          else if(i%2==1){
            leds[i + 10 - 1] = Red;}
        } else if (!(CurrB & _BV(i))) {
          NKROKeyboard.release(KeyCode[i + 10]);
          if(i%2==0){
            leds[i + 10] = Aqua;}
          else if(i%2==1){
            leds[i + 10] = Red;}
        }
      }
      change = true;
      LastB = CurrB;
    }
     CurrC = capC.touched();
    if (CurrC != LastC) {
      for (uint8_t i = 0; i < 10; i++) {
        if ((CurrC & _BV(i))) {
          NKROKeyboard.press(KeyCode[i]);
          if(i%2==0){
            leds[i + 22] = Red;}
          else if(i%2==1){
            leds[i + 22 - 1] = Red;}
        } else if (!(CurrC & _BV(i))) {
          NKROKeyboard.release(KeyCode[i]);
          if(i%2==0){
            leds[i + 22] = Aqua;}
          else if(i%2==1){
            leds[i + 22] = Red;}
        }
      }
      change = true;
      LastC = CurrC;
    }
    if (change) {
      NKROKeyboard.send();
      FastLED[0].show(leds, LED_NUM, LED_BRI);
    }
    AirCheck();
}

//////启动函数//////

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

  Wire.setClock(800000);//如果感觉有明显延迟，调成1600000
}


void setup() {
  //串口启动//
  SerialUSB.begin(115200);
  SerialUSB.setTimeout(0);
  //灯带添加//
  LEDset();
  TouchSetup();//触摸启动
  AirSetup();//Air启动
  AirCalibrate();//Air校准
}



void loop() {
  MprCheck();
}


