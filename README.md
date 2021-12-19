# LINGNITHM
个人记录向半开源CHUNITHM风格音乐游戏控制器

# 如果需要制作本控制器，你大体需要以下能力
1：完善的文件处理能力，可以阅读并看懂包括但不限于CAD，STL，Gerber等文件的作用
2：一定的C++语言知识，可以阅读并大致理解固件函数作用
3：一定的动手能力，能完成包括但不限于拧螺丝，贴片焊接，调试硬件等能力

# 控制器构成
控制器基于Arduino IDE，使用第三方的platfromIO编写固件程序

电路主控芯片使用了ATMEGA SAMD21 MCU，触摸IC使用了MPR121，AIR抬手感应使用了红外对射灯珠

基本连接思路为 主控板-触摸板   主控板-AIR发射/接收转接器-HDMI线材-AIR发射/接收
                
固件方面  
MPR121调参库使用了@ilufang的MPR121库

其余方面想到再补充（
