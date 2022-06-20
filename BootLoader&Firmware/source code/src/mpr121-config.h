#ifndef _MPR121_CONFIG_H
#define _MPR121_CONFIG_H

#include "mpr121-helper.h"

// Thresholds
#define MPR121CONF_TTH  6///< 触发阈值
#define MPR121CONF_RTH  3///< 释放阈值
// De-bounce
#define MPR121CONF_DR 3 ///< 松开样本采集数
#define MPR121CONF_DT 2 ///< 按下样本采集数
// AFE Configurations
#define MPR121CONF_FFI  MPR121_FFI_6 ///< 一层过滤采样数
#define MPR121CONF_CDC  16 ///< 充放电电流
#define MPR121CONF_CDT  MPR121_CDT_500NS ///< 充放电时间
#define MPR121CONF_SFI  MPR121_SFI_4 ///< 二层过滤
#define MPR121CONF_ESI  MPR121_ESI_1MS ///< 二层过滤采样间隔时间
// ECR Configurations
#define MPR121CONF_CL   MPR121_CL_INIT1 ///< 启用基线动态校准
#define MPR121CONF_ELEPROX  MPR121_ELEPROX_0  ///< 接近检测电极，禁用
#define MPR121CONF_ELE    12 ///< touch detection electrodes
// 基线动态校准
#define MPR121CONF_MHDR 1 ///< 上升最大变化值
#define MPR121CONF_NHDR 8 ///< 上升幅度
#define MPR121CONF_NCLR 1 ///< 上升修正样本个数
#define MPR121CONF_FDLR 0 ///< 修正前等待样本个数
#define MPR121CONF_MHDF 1 ///< 下降最大变化值
#define MPR121CONF_NHDF 1 ///< 下降幅度
#define MPR121CONF_NCLF 16  ///< 下降修正样本个数
#define MPR121CONF_FDLF 2 ///< 修正前等待样本个数
#define MPR121CONF_NHDT 0 ///< Touched Noise Half Delta
#define MPR121CONF_NCLT 0 ///< Touched Noise Count Limit
#define MPR121CONF_FDLT 0 ///< Touched Filter Delay Count Limit
// 自动初始化
// #define MPR121CONF_USL    200 ///< 上限，MPR121_UPLIMIT，((Vdd - 0.7)/Vdd) * 256，200
// #define MPR121CONF_LSL    130 ///< 下限，MPR121_LOWLIMIT，UPLIMIT * 0.65，130
// #define MPR121CONF_TL   180 ///< 目标，MPR121_TARGETLIMIT，UPLIMIT * 0.9，180

#define MPR121CONF_USL    156 ///< 上限，MPR121_UPLIMIT，((Vdd - 0.7)/Vdd) * 256
#define MPR121CONF_LSL    101 ///< 下限，MPR121_LOWLIMIT，UPLIMIT * 0.65
#define MPR121CONF_TL   140 ///< 目标，MPR121_TARGETLIMIT，UPLIMIT * 0.9

// #define MPR121CONF_USL    166 ///< 上限，MPR121_UPLIMIT，((Vdd - 0.7)/Vdd) * 256，200
// #define MPR121CONF_LSL    108 ///< 下限，MPR121_LOWLIMIT，UPLIMIT * 0.65，130
// #define MPR121CONF_TL   149 ///< 目标，MPR121_TARGETLIMIT，UPLIMIT * 0.9，180

#define MPR121CONF_AFES   MPR121CONF_FFI  ///< 必须设置为FFI
#define MPR121CONF_RETRY  MPR121_RETRY_0  ///< 重试次数
#define MPR121CONF_BVA    MPR121CONF_CL ///< 必须设置为CL
#define MPR121CONF_ARE    1 ///< 开启自动重配置
#define MPR121CONF_ACE    1 ///< 开启自动配置
#define MPR121CONF_SCTS   1 ///< 跳过充电时间
//中断用，全部关闭
#define MPR121CONF_ACFIE  0 ///< Auto-configuration fail interrupt enable
#define MPR121CONF_ARFIE  0 ///< Auto-reconfiguration fail interrupt enable
#define MPR121CONF_OORIE  0 ///< Out-of-range interrupt enable

#endif
