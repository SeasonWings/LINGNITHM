# LINGNITHM
个人记录向半开源CHUNITHM风格音乐游戏控制器

# 如果需要制作本控制器，你大体需要以下能力
1：完善的文件处理能力，可以阅读并看懂包括但不限于CAD，STP，Gerber等文件的作用
2：一定的C++语言知识，可以阅读并大致理解固件函数作用
3：一定的动手能力，能完成包括但不限于拧螺丝，贴片焊接，调试硬件等能力

# 控制器构成
控制器基于Arduino IDE，使用第三方的platfromIO编写固件程序

电路主控芯片使用了ATMEGA SAMD21 MCU，触摸IC使用了MPR121，AIR抬手感应使用了940MM红外对射灯珠

基本连接思路为 主控板-触摸板   主控板-AIR发射/接收转接器-HDMI线材-AIR发射/接收

如图所示
![](https://github.com/SeasonWings/LINGNITHM/blob/main/picture/%E5%86%85%E9%83%A8%E6%9E%84%E9%80%A0.jpg)
                
固件方面  
有参考GSK，XPENG等大佬的固件，在此提出感谢！

上传方面
你需要购买Jlink上传器，通过SEGGER软件向SAMD21上传固件以及BootLoader

物料方面
请在制作完PCB之后自行对照丝印（其实是我懒得整理BOM）

其余方面想到再补充（


PCB1.1更新，修复几个错误
新上传一个钣金外壳，CAD零件待补充


1.0更新
  PCB Gerber文件不变，钣金外壳得以完善。
  现在完全否定了打印外壳的想法啦！

# 外观展示
![](https://github.com/SeasonWings/LINGNITHM/blob/main/picture/%E5%A4%96%E8%A7%821.jpg)
