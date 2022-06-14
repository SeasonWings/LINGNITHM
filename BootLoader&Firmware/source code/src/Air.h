////基础定义////
#include "HID-Project.h"
#include "FastLED.h"
#include <Arduino.h>
#define Serial SerialUSB

////Air定义////
int calibrationCounter;
bool calibrated;
uint16_t thresholds[6];
uint16_t maxReadings[6];

#define AIR_LED_DELAY 96//轮询延迟
#define AIR_INPUT_SENSITIVITY 0.3//红外敏感度
#define CALIBRATION_SAMPLES 256//校准采样数
#define SKIP_SAMPLES 256//校准跳过采样数

int ir_input_pins[6] = {3,4,5,2,6,11};//定义轮询发射红外针脚
int ir_sensor_pins[6] = {A0,A1,A2,A3,A4,A5};//定义轮询接收红外针脚
////按键定义//// 
KeyboardKeycode AirKeys[6] = {KEY_SLASH, KEY_PERIOD, KEY_QUOTE, KEY_SEMICOLON, KEY_RIGHT_BRACE, KEY_LEFT_BRACE};
uint8_t AirState[6];
////灯光定义////
#define air_right_pin 8 //R灯引脚
#define air_right_num 6 //R灯数量
CRGB AIR_RIGHT[air_right_num];

#define air_left_pin 10 //L灯引脚
#define air_left_num 6 //L灯数量
CRGB AIR_LEFT[air_left_num];


uint8_t colorIndex;

void CloudboardAirLED(){//键盘Air灯效1 
  colorIndex++;
  fill_palette(AIR_LEFT, 6 , colorIndex, 5, CloudColors_p, 100, LINEARBLEND);
  fill_palette(AIR_RIGHT, 6 , colorIndex, 5, CloudColors_p, 100, LINEARBLEND);
  for (uint8_t i = 0; i < 6; i++) {   
    if (AirState[i]) {
      AIR_RIGHT[i] = CRGB::White;
      AIR_LEFT[i] = CRGB::White;
  }
  FastLED[1].show(AIR_RIGHT,6, 150);
  FastLED[2].show(AIR_LEFT,6, 150);
  }
}

//////////Air//////////
void AirSetup() {
  calibrated = false;
  for (int i = 0; i < 6; i++) {
    maxReadings[i] = 0;
  }
  for (int i = 0; i < 6; i++) {
    pinMode(ir_sensor_pins[i], INPUT_PULLUP);
    pinMode(ir_input_pins[i], OUTPUT);
    AirState[i] = 0;
  }
}

void turnOffLight() {//关闭发射函数
  for(int i = 0;i < 6;i++){
    digitalWrite(ir_input_pins[i], LOW);
    }
}

void changeLight(int light) {//红外发射轮询
  switch (light) {
    case 0:
      digitalWrite(ir_input_pins[light], HIGH);//#1
      break;
    case 1:
      digitalWrite(ir_input_pins[light], HIGH);//#2
      break;
    case 2:
      digitalWrite(ir_input_pins[light], HIGH);//#3
      break;
    case 3:
      digitalWrite(ir_input_pins[light], HIGH);//#4
      break;
    case 4:
      digitalWrite(ir_input_pins[light], HIGH);//#5
      break;
    case 5:
      digitalWrite(ir_input_pins[light], HIGH);//#6
      break;
    default:
      turnOffLight();
      break;
  }
}

uint16_t getValue(int sensor) {//获取开灯数值函数
  changeLight(sensor);// 开灯
  delayMicroseconds(AIR_LED_DELAY/2);// 加上延迟保证完全开启
  return analogRead(ir_sensor_pins[sensor]);//读数值
}

bool getSensorState(int sensor) {//返回Air状态
  turnOffLight();//关灯
  delayMicroseconds(AIR_LED_DELAY/2);//加上延迟保证完全关闭
  int enviroment = analogRead(ir_sensor_pins[sensor]);//读取环境数值
  int value = getValue(sensor);//读取开灯数值
  
  //调试AIR用输出
  // Serial.print(sensor);
  // Serial.print("\t");
  // Serial.print(value);
  // Serial.print("\t");
  // Serial.print(enviroment);
  // Serial.print("\t");
  // Serial.print(value - enviroment);
  // Serial.print("\t");
  // Serial.print(value - enviroment + 200);
  // Serial.print("\t");
  // Serial.print(maxReadings[sensor]);
  // Serial.print("\t");
  // Serial.print(thresholds[sensor]);
  // Serial.println();
  
  //把读数减去环境数值和校准数值比较后返回布尔值，+200为防止负数的比较
  return (value - enviroment + 200) < (thresholds[sensor] + 200);
  
}

void AirCalibrate() {//校准接收端
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
        delayMicroseconds(AIR_LED_DELAY/2);//加上延迟保证完全关闭
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
        AirState[i] = 1;
      } else if (!getSensorState(i)){
        NKROKeyboard.release(AirKeys[i]);
        AirState[i] = 0;
      }
    }
  }
}

void NoAirCheck(){
  if (calibrated){
    for (int i = 0; i < 6; i++){
      if (getSensorState(i)){
        AirState[i] = 1;
      } else if (!getSensorState(i)){
        AirState[i] = 0;
      }
    }
  }
}
