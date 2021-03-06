# LINGNITHM
个人记录向半开源CHUNITHM风格音乐游戏控制器

# 如果需要制作本控制器，你大体需要以下能力
* 完善的文件处理能力，可以阅读并看懂包括但不限于CAD，STP，Gerber等文件的作用
* 一定的单片机知识，能够识别和使用BootLoader和固件bin文件
* 一定的动手能力，能完成包括但不限于拧螺丝，贴片焊接，调试硬件等能力

# 控制器构成
* 控制器基于Arduino IDE，使用第三方的platfromIO编写固件程序
* 电路主控芯片使用了ATMEGA SAMD21G18A MCU，触摸IC使用了MPR121，AIR抬手感应使用了940MM红外对射灯珠
* 基本连接思路为 主控板-触摸板   主控板-AIR发射/接收转接器-HDMI线材-AIR发射/接收

如图所示
![](https://github.com/SeasonWings/LINGNITHM/blob/main/picture/%E5%86%85%E9%83%A8%E6%9E%84%E9%80%A0.jpg)
                
* [固件方面](https://github.com/SeasonWings/LINGNITHM/tree/main/BootLoader%26Firmware) 有参考GSK，XPENG，ilufang，Sucareto等大佬的代码，在此特别提出感谢！！！
* 上传方面 你需要购买Jlink上传器，通过SEGGER软件向SAMD21上传固件以及BootLoader
* [物料方面](https://github.com/SeasonWings/LINGNITHM/blob/main/PCB%20Gerber%E5%88%B6%E7%89%88%E6%96%87%E4%BB%B61.1/PCB%E7%89%A9%E6%96%99BOM%E8%A1%A8.xlsx) 请在制作完PCB之后自行对照丝印(此句划掉，现在有了)

其余方面想到再补充(

# Update

#### 1.0更新
  * 脱胎换骨！现在你可以使用金属化的控制器外壳了！
  * 新的PCB设计改动以适配新的外壳。
  * 新的固件，包含直连协议(需要配合[修改后的chuniio和Segatools.ini](https://github.com/SeasonWings/LINGNITHM/tree/main/BootLoader%26Firmware/1.0%20Ver/chuniio%26Segatools "请务必将你的控制器设备放在COM1端口上！")使用),以及更加炫酷的灯光，更优的触摸算法。
  * 最新最热PCB物料BOM表已更新，感谢群友海腾的帮助！

#### 2.0更新
  * 通过上传新的固件&BootLoader来获得更新功能！
  * 键盘模式下新增设置接口，同时遮挡2，4，6号Air即可进入设置模式
  * 新增设置模式下的操作逻辑，稍后使用PDF文件进行说明
  * 现在你可以为你的键盘以及Air灯光效果自由编辑喜欢的灯光样式啦！

#### 3.0更新
  * LINGCON Setting的操作UI有部分变动
  * Setting进入逻辑修改，改变为全押Air

### 久违的更新（
#### 4.0更新
  * 代码触摸逻辑有大幅改动，现在触摸或许会更灵敏了(?)
  * Setting新增触摸校准（黄色按钮），把手指用你认为最小的力度划过触摸区来完成校准
  * 改变了新的上传Firmware方式，现在更加方便了,旧固件停止维护
  * 同时公开部分底层源代码，有能人士可以按自己所需添加想要的功能，详情见Source code

# License
CC
  * 你可以在遵循CC协议的条件下自由修改，查看，下载，使用本项目内文件
  * 禁止进行一切商业行为（“商业行为”指一切涉及本项目资金流动，包括但不限于：直接clone本项目文件 不经任何修改 拿来以任何方式出售或牟利的行为）
  * 最终解释权归本人所有
# 外观展示
![](https://github.com/SeasonWings/LINGNITHM/blob/main/picture/%E5%A4%96%E8%A7%821.jpg)

# 控制器使用展示
[点击这里！](https://www.bilibili.com/video/BV1Qm4y1Z7ms/ "你会给两个圆圆的硬币对么？")

如果觉得非常棒，可以给我一个一键三连么(
