#include "Touch.h"
#define Serial SerialUSB

////核心函数////
static void slider_set_led() {//串口读取led数据
  if (slider_tx_pending) {    
    return;
  }
  for (int i = 0; i < 31; i++) {//BRG
      uint8_t t1 = i;
      uint8_t t2 = i * 3;
      leds[t1].r = slider_req.leds[t2 + 1];
      leds[t1].g = slider_req.leds[t2 + 2];
      leds[t1].b = slider_req.leds[t2 + 0];
    }
  FastLED[0].show(leds, LED_NUM, LED_BRI);
  slider_req.cmd = 0; 
}


static void slider_scan() {//直连触摸扫描
  if (!auto_scan || slider_tx_pending) {//分支选择，非直连状态
    TouchCheck();//各状态的触摸板检测
    NoAirCheck();//无HID输入的Air
    AirLED();
    return;
  }//分支选择，直连状态
  slider_resp.syn = 0xff;
  slider_resp.cmd = SLIDER_CMD_AUTO_SCAN;
  slider_resp.size = sizeof(slider_resp.pressure);
  int16_t bv, fd, clp;
  for (int i = 0; i < 12; i++) {//直连触摸数值计算
    if (i < 10) {
      bv = capA.baselineData(i);
      fd = capA.filteredData(i);
      clp = CalCheck(bv, fd);
      slider_resp.pressure[i] = CLAMP(clp, 0, 255);
    }
      bv = capB.baselineData(i);
      fd = capB.filteredData(i);
      clp = CalCheck(bv, fd);
      slider_resp.pressure[i + 10] = CLAMP(clp, 0, 255);
    if (i < 10) {
      bv = capC.baselineData(i);
      fd = capC.filteredData(i);
      clp = CalCheck(bv, fd);
      slider_resp.pressure[i + 10 + 12] = CLAMP(clp, 0, 255);
    }
  }  
    AirCheck();//有HID输入的Air
    AirLED();
  }
  
static void slider_scan_start() {//启动slider
  auto_scan = true;
  slider_scan();
}
static void slider_scan_stop() {//停止slider
  auto_scan = false;
  slider_resp.syn = 0xff;
  slider_resp.cmd = SLIDER_CMD_AUTO_SCAN_STOP;
  slider_resp.size = 0;
  slider_req.cmd = 0;
}
static void slider_reset() {//重置slider，重新初始化触摸
  capA.init();
  capA.run();
  capB.init();
  capB.run();
  capC.init();
  capC.run();
  slider_resp.syn = 0xff;
  slider_resp.cmd = SLIDER_CMD_RESET;
  slider_resp.size = 0;
  slider_req.cmd = 0;
}
static void slider_get_board_info() {//返回版本信息，勿修改
  slider_resp.syn = 0xff;
  slider_resp.cmd = SLIDER_CMD_GET_BOARD_INFO;
  slider_resp.size = sizeof(slider_resp.version);
  strcpy(slider_resp.version, "15330   \xA0""06712\xFF""\x90");
  slider_req.cmd = 0;
}

void SerialCheck() {//串口模式的loop函数
  switch (sliderserial_readreq()) {
    case SLIDER_CMD_SET_LED:
      slider_set_led();
      break;
    case SLIDER_CMD_AUTO_SCAN_START:
      slider_scan_start();
      break;
    case SLIDER_CMD_AUTO_SCAN_STOP:
      slider_scan_stop();
      break;
    case SLIDER_CMD_RESET:
      slider_reset();
      break;
    case SLIDER_CMD_GET_BOARD_INFO:
      slider_get_board_info();
      break;
    default:
      slider_scan();
  }
  sliderserial_writeresp();
}

////主进程函数////
void setup() {
  //串口启动//
  SerialUSB.begin(115200);
  SerialUSB.setTimeout(0);

  LEDset();//灯带添加
  TouchSetup();//触摸启动
  AirSetup();//Air启动
  AirLED = CloudboardAirLED;
  TouchCheck = K32Check;
  TouchLEDCheck = NoTouchLED;
  AirCalibrate();//Air校准  
}

void loop() {
  SerialCheck();//主循环
}
