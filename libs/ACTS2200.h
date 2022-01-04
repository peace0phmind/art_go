#ifndef _ART_ACTS2200_H_
#define _ART_ACTS2200_H_

#include <stdint.h>

#define DEV_MAX                 64                              // 驱动支持最大设备数

// #################### 人性化的数据类型定义 #####################
// Humanized data types definition
typedef uint8_t     UINT8;
typedef uint16_t    UINT16;
typedef uint32_t    UINT32;
typedef uint64_t    UINT64;
typedef int8_t      INT8;
typedef int16_t     INT16;
typedef int32_t     INT32;
typedef int64_t     INT64;
typedef float       F32;
typedef double      F64;
typedef void*       HANDLE;
typedef int         BOOL;
typedef void*       PVOID;

typedef INT8		I8;
typedef UINT8		U8;
typedef INT16		I16;
typedef UINT16		U16;
typedef INT32		I32;
typedef UINT32		U32;
typedef INT64		I64;
typedef UINT64      U64;

//////////////////////////////////////////////////////////////////
#define TRUE        true
#define FALSE       false
#define INVALID_HANDLE_VALUE (void*)-1

// ########################## 设备功能指标 ########################
#define ACTS2200_AI_MAX_CHANNELS                    32          // 本程序最多支持64路模拟量单端输入通道
#define ACTS2200_AO_MAX_CHANNELS                    4           // 本程序最多支持4个模拟量输出通道
#define ACTS2200_DIO_MAX_PORTS                      2           // 本程序最多支持2个数字量端口
#define ACTS2200_CTR_MAX_CHANNELS                   2           // 本程序最多支持2个计数器

#define ACTS2200_AO_DATA_WIDTH                      2           // AO采样数据宽度(字节数)
#define ACTS2200_DI_DATA_WIDTH                      1           // DI采样数据宽度(字节数)
#define ACTS2200_DO_DATA_WIDTH                      1           // DO采样数据宽度(字节数)
#define ACTS2200_CI_DATA_WIDTH                      4           // CI采样数据宽度(字节数)
#define ACTS2200_CO_DATA_WIDTH                      8           // CO采样数据宽度(字节数)

///////////////////// 信号导出 ////////////////////////
//ACTS2200_ExportSignal函数nSignalID所使用的常量定义
#define ACTS2200_VAL_NONE                           0           // 配置此值时，接线端停止输出之前导出的信号，并保持在最后一个时刻的电平状态上
#define ACTS2200_VAL_AIConvertClock                 0x0301      // AI采集任务的转换时钟，当选择多个AI通道时，输出的时钟为多个通道的总采样时钟
#define ACTS2200_VAL_AOSampleClock                  0x0400      // AO输出任务的采样时钟，时钟频率为采样任务使用的时钟频率
#define ACTS2200_VAL_CO0_Output                     0x0020      // CO0脉冲输出，默认为对应计数器通道的输出接线端，可通过ExportSignal指定输出接线端
#define ACTS2200_VAL_CO1_Output                     0x0021      // CO1脉冲输出，默认为对应计数器通道的输出接线端，可通过ExportSignal指定输出接线端
#define ACTS2200_VAL_CTR0OutputEvent                0x0030      // 计数器0输出事件，默认为对应计数器通道的输出接线端，可通过ExportSignal指定输出接线端
#define ACTS2200_VAL_CTR1OutputEvent                0x0031      // 计数器1输出事件，默认为对应计数器通道的输出接线端，可通过ExportSignal指定输出接线端

//ACTS2200_ExportSignal函数nTerm所使用的常量定义
#define ACTS2200_VAL_PFI0                           0x110       // 接线端PFI0
#define ACTS2200_VAL_PFI1                           0x111       // 接线端PFI1
#define ACTS2200_VAL_PFI2                           0x112       // 接线端PFI2
#define ACTS2200_VAL_PFI3                           0x113       // 接线端PFI3
#define ACTS2200_VAL_PFI4                           0x114       // 接线端PFI4
#define ACTS2200_VAL_PFI5                           0x115       // 接线端PFI5
#define ACTS2200_VAL_PFI6                           0x116       // 接线端PFI6
#define ACTS2200_VAL_PFI7                           0x117       // 接线端PFI7


///////////////////// AI ////////////////////////
// ############# AI工作参数结构体ACTS2200_AI_PARAM描述 #############
typedef struct _ACTS2200_AI_CH_PARAM // AI通道参数结构体
{
    U32 nChannel;                                               // 通道号
    U32 nSampRange;                                             // 采样范围(Sample Range)档位选择, 具体见下面常量名定义(此参数在单点采样有效，未此注明的参数则在单点采样模式中无效)
    U32 nRefGround;                                             // 地参考方式

    U32 nReserved0;                                             // 保留(未用)
    U32 nReserved1;                                             // 保留(未用)
    U32 nReserved2;                                             // 保留(未用)
} ACTS2200_AI_CH_PARAM, *PACTS2200_AI_CH_PARAM;

// AI硬件参数结构体ACTS2200_AI_CH_PARAM中的nSampRange参数所使用的采样范围挡位选项
#define ACTS2200_AI_SAMPRANGE_N10_P10V              0           // ±10V
#define ACTS2200_AI_SAMPRANGE_N5_P5V                1           // ±5V
#define ACTS2200_AI_SAMPRANGE_N2_P2V                2           // ±2V
#define ACTS2200_AI_SAMPRANGE_N1_P1V                3           // ±1V
#define ACTS2200_AI_SAMPRANGE_N2P5_P2P5V            4           // ±2.5V 保留(未用)
#define ACTS2200_AI_SAMPRANGE_0V_P10V               5           // 0-10V  保留(未用)
#define ACTS2200_AI_MAX_RANGES                      6

// AI硬件通道参数结构体ACTS2200_AI_CH_PARAM中的nRefGround参数所使用的地参考选项
#define ACTS2200_AI_REFGND_RSE                      0           // 接地参考单端(Referenced Single Endpoint)
#define ACTS2200_AI_REFGND_NRSE                     1           // 非参考单端(Non Referenced Single Endpoint)
#define ACTS2200_AI_REFGND_DIFF                     2           // 差分(Differential)

typedef struct _ACTS2200_AI_START_TRIG                          // 开始触发参数
{
    U32 nTrigType;                                              // 触发类型
    U32 nTrigSrc;                                               // 触发源, 详见下面常量定义
    U32 nTrigDir;                                               // 触发方向
    U32 bReTrig;                                                // 重触发使能, 1:使能
    F32 fTrigLvlTop;                                            // 触发窗顶部(边沿触发电平),单位:伏(V)
    F32 fTrigLvlBtm;                                            // 触发窗底部,单位:伏(V)
    F64 fTrigDigFltrMinPulseWidth;                              // 触发数字滤波最小脉冲宽度，单位秒(S)
    F64 fDelaySeconds;                                          // 触发延迟时间，单位秒(S)
    U32 nReserved0;                                             // 保留字段(暂未定义)
    U32 nReserved1;                                             // 保留字段(暂未定义)
} ACTS2200_AI_START_TRIG, *PACTS2200_AI_START_TRIG;

typedef struct _ACTS2200_AI_REF_TRIG
{
    U32 nTrigType;                                              // 触发类型
    U32 nTrigSrc;                                               // 触发源, 详见下面常量定义
    U32 nTrigDir;                                               // 触发方向
    U32 nPretrigSamps;                                          // 触发前预采样点数
    F32 fTrigLvlTop;                                            // 触发窗顶部(边沿触发电平),单位:伏(V)
    F32 fTrigLvlBtm;                                            // 触发窗底部,单位:伏(V)
    F64 fTrigDigFltrMinPulseWidth;                              // 触发数字滤波最小脉冲宽度，单位秒(S)
    U32 nReserved0;                                             // 保留字段(暂未定义)
    U32 nReserved1;
} ACTS2200_AI_REF_TRIG, *PACTS2200_AI_REF_TRIG;

typedef struct _ACTS2200_AI_PAUSE_TRIG                          // 暂停触发参数
{
    U32 nTrigType;                                              // 触发类型,
    U32 nTrigSrc;                                               // 触发源, 详见下面常量定义
    U32 nTrigDir;                                               // 触发方向
    U32 nReserved0;                                             // 保留字段(暂未定义)
    F32 fTrigLvlTop;                                            // 触发窗顶部(边沿触发电平),单位:伏(V)
    F32 fTrigLvlBtm;                                            // 触发窗底部,单位:伏(V)
    F64 fTrigDigFltrMinPulseWidth;                              // 触发数字滤波最小脉冲宽度，单位秒(S)
    U32 nReserved1;                                             // 保留字段(暂未定义)
    U32 nReserved2;                                             // 保留字段(暂未定义)
} ACTS2200_AI_PAUSE_TRIG, *PACTS2200_AI_PAUSE_TRIG;

// AI开始触发参数结构体ACTS2200_AI_START_TRIG中的nTrigType触发类型所使用的选项
#define ACTS2200_VAL_NONE                           0           // 无触发(等同于软件强制触发)
#define ACTS2200_VAL_ANALOG_EDGE                    1           // 模拟边沿触发类型
#define ACTS2200_VAL_ANALOG_WIN                     2           // 模拟窗触发类型
#define ACTS2200_VAL_DIGIT_EDGE                     3           // 数字边沿触发类型
#define ACTS2200_VAL_DIGIT_PATTERN                  4           // 数字模式触发类型(保留,未提供)

// AI暂停触发参数结构体ACTS2200_AI_PAUSE_TRIG中的nTrigType触发类型所使用的选项
#define ACTS2200_VAL_NONE                           0           // 无触发(禁用暂停触发)
#define ACTS2200_VAL_ANALOG_LVL                     1           // 模拟电平发类型
#define ACTS2200_VAL_ANALOG_WIN                     2           // 模拟窗触发类型
#define ACTS2200_VAL_DIGIT_LVL                      3           // 数字电平触发类型
#define ACTS2200_VAL_DIGIT_PATTERN                  4           // 数字模式触发类型(保留,未提供)

// AI开始触发参数结构体ACTS2200_AI_START_TRIG中的nTrigSrc触发源所使用的选项
// AI开始触发参数结构体ACTS2200_AI_PAUSE_TRIG中的nTrigSrc触发源所使用的选项
#define ACTS2200_VAL_AIFirst                        0x100       // 模拟触发输入接线端，采样通道阵列中的第一个模拟量输入通道(仅模拟触发有效)
#define ACTS2200_VAL_APFI                           0x101       // 模拟触发输入接线端，专用ATR引脚,由设备连接器外部输入(保留，本设备不支持)
#define ACTS2200_VAL_PFI0                           0x110       // 接线端PFI0
#define ACTS2200_VAL_PFI1                           0x111       // 接线端PFI1
#define ACTS2200_VAL_PFI2                           0x112       // 接线端PFI2
#define ACTS2200_VAL_PFI3                           0x113       // 接线端PFI3
#define ACTS2200_VAL_PFI4                           0x114       // 接线端PFI4
#define ACTS2200_VAL_PFI5                           0x115       // 接线端PFI5
#define ACTS2200_VAL_PFI6                           0x116       // 接线端PFI6
#define ACTS2200_VAL_PFI7                           0x117       // 接线端PFI7

// AI开始触发参数结构体ACTS2200_AI_START_TRIG中的nTrigDir触发方向所使用的选项
// 当触发类型为模拟边沿触发或数字边沿触发时(nTrigType = TRIGTYPE_ANALOG_EDGE/TRIGTYPE_DIGIT_EDGE):
#define ACTS2200_VAL_FALLING                        0           // 下降沿
#define ACTS2200_VAL_RISING                         1           // 上升沿
#define ACTS2200_VAL_CHANGING                       2           // 变化(即上下边沿均有效)

// 当触发类型为模拟窗触发(nTrigType = TRIGTYPE_ANALOG_WIN):
#define ACTS2200_VAL_EnteringWin                    0           // 入窗
#define ACTS2200_VAL_LeavingWin                     1           // 出窗
#define ACTS2200_VAL_LeavingEnterWin                2           // 变化(出入窗均有效)

// AI暂停触发参数结构体ACTS2200_AI_PAUSE_TRIG中的nTrigDir触发方向所使用的选项
// 当触发类型为模拟电平触发或数字电平触发时(nTrigType = TRIGTYPE_ANALOG_LVL/TRIGTYPE_DIGIT_LVL):
#define ACTS2200_VAL_LOW                            0           // 低电平
#define ACTS2200_VAL_HIGH                           1           // 高电平

// 当触发类型为模拟窗触发(nTrigType = TRIGTYPE_ANALOG_WIN):
#define ACTS2200_VAL_InsideWin                      0           // 窗内
#define ACTS2200_VAL_OutsideWin                     1           // 窗外

typedef struct _ACTS2200_AI_PARAM                               // 跟整个AI有关的硬件工作参数(简称AI工作参数)
{
    // 通道参数
    U32 nSampChanCount;                                         // 采样通道总数[1, 4](此参数属于只读参数，仅在AI_InitTask()中根据CHParam中的bChannelEn的设置值统计实际采样通道数量并通过它返回给用户使用)
    U32 nSampSignal;                                            // 采样信号(Sample Signal), 详见下面常量定义
    U32 nReserved0;                                             // 保留字段(暂未定义)
    U32 nReserved1;                                             // 保留字段(暂未定义)
    ACTS2200_AI_CH_PARAM CHParam[ACTS2200_AI_MAX_CHANNELS];     // 通道参数配置

    // 时钟参数
    U32 nSampMode;                                              // 采样模式, 0=单点采样(按需)， 1、硬件定时单点(暂不支持)，2=有限点采样， 3=连续采样
    U32 nSampsPerChan;                                          // 每个通道采样点数(也是每通道待读取点数),取值范围为单点采样模式时应等于1，有限点或连续采样时为[2, 1024*1024]
    F64 fSampRate;                                              // 采样速率(Sample Rate), 单位为sps，取值范围为[0, 500K sps],它是每个采样通道的采样速率。
    U32 nSampClkSrc;                                            // 采样时钟源(请详见下面的常量定义)
    U32 nExtSampClkEdge;                                        // 外部采样时钟边沿, 0=下降沿, 1=上升沿(仅当nSampClkSrc外部采样时钟源时有效)

    ACTS2200_AI_START_TRIG StartTrig;                           // 开始触发参数
    ACTS2200_AI_PAUSE_TRIG PauseTrig;                           // 暂停触发参数
    ACTS2200_AI_REF_TRIG   RefTrig;                             // 参考触发参数

    // 其他方面
    U32 nReserved2;                                             // 保留字段(暂未定义)
    U32 nReserved3;                                             // 保留字段(暂未定义)
} ACTS2200_AI_PARAM, *PACTS2200_AI_PARAM;

// AI硬件参数结构体ACTS2200_AI_PARAM中的nSampSignal采样信号所使用的选项
#define ACTS2200_AI_SAMPSIGNAL_AI                   0           // AI通道输入信号
#define ACTS2200_AI_SAMPSIGNAL_0V                   1           // 0V(AGND)
#define ACTS2200_AI_SAMPSIGNAL_4D096V               2           // 4.096V
#define ACTS2200_AI_SAMPSIGNAL_N4D096V              3           // -4.096V
#define ACTS2200_AI_SAMPSIGNAL_AO0                  4           // AO0
#define ACTS2200_AI_SAMPSIGNAL_NAO0                 5           // -AO0
#define ACTS2200_AI_SAMPSIGNAL_AO1                  6           // AO1
#define ACTS2200_AI_SAMPSIGNAL_NAO1                 7           // -AO1
#define ACTS2200_AI_SAMPSIGNAL_AO2                  8           // AO2
#define ACTS2200_AI_SAMPSIGNAL_NAO2                 9           // -AO2
#define ACTS2200_AI_SAMPSIGNAL_AO3                  10          // AO3
#define ACTS2200_AI_SAMPSIGNAL_NAO3                 11          // -AO3
#define ACTS2200_AI_SAMPSIGNAL_COUNT                12

// AI硬件参数结构体ACTS2200_AI_PARAM中的nSampClkSrc采样时钟源所使用的选项
#define ACTS2200_VAL_LOCAL                          0           // 使用本地源
#define ACTS2200_VAL_PFI0                           0x110       // 接线端PFI0
#define ACTS2200_VAL_PFI1                           0x111       // 接线端PFI1
#define ACTS2200_VAL_PFI2                           0x112       // 接线端PFI2
#define ACTS2200_VAL_PFI3                           0x113       // 接线端PFI3
#define ACTS2200_VAL_PFI4                           0x114       // 接线端PFI4
#define ACTS2200_VAL_PFI5                           0x115       // 接线端PFI5
#define ACTS2200_VAL_PFI6                           0x116       // 接线端PFI6
#define ACTS2200_VAL_PFI7                           0x117       // 接线端PFI7

// AI硬件参数结构体ACTS2200_AI_PARAM中的nExtSampClkEdge外部采样时钟边沿所使用的选项
#define ACTS2200_VAL_FALLING                        0           // 下降沿
#define ACTS2200_VAL_RISING                         1           // 上升沿


// #################### AI工作状态结构体ACTS2200_AI_STATUS描述 #####################
typedef struct _ACTS2200_AI_STATUS
{
    U32 bTaskDone;                                              // 采集任务完成标志, =TRUE:表示采集任务完成, =FALSE:表示采集任务未完成，正在进行中
    U32 bTriged;                                                // 触发标志, =TRUE:表示已被触发, =FALSE:表示未被触发(即正等待触发)

    U32 nTaskState;                                             // 采集任务状态, =1:正常, 其它值表示有异常情况
    U32 nAvailSampsPerChan;                                     // 每通道有效点数，只有它大于当前指定读数长度时才能调用AI_ReadAnalog()立即读取指定长度的采样数据
    U32 nMaxAvailSampsPerChan;                                  // 自AI_StartTask()后每通道出现过的最大有效点数，状态值范围[0, nBufSampsPerChan],它是为监测采集软件性能而提供，如果此值越趋近于1，则表示意味着性能越高，越不易出现溢出丢点的可能
    U32 nBufSampsPerChan;                                       // 每通道缓冲区大小(采样点数)。
    U64 nSampsPerChanAcquired;                                  // 每通道已采样点数(自开始采集任务(AI_StartTask())之后所采样的点数)，这个只是给用户的统计数据

    U32 nHardOverflowCnt;                                       // 硬件溢出计数(在不溢出情况下恒等于0)
    U32 nSoftOverflowCnt;                                       // 软件溢出计数(在不溢出情况下恒等于0)
    U32 nInitTaskCnt;                                           // 初始化采集任务的次数(即调用AI_InitTask()的次数)
    U32 nReleaseTaskCnt;                                        // 释放采集任务的次数(即调用AI_ReleaseTask()的次数)
    U32 nStartTaskCnt;                                          // 开始采集任务的次数(即调用AI_StartTask()的次数)
    U32 nStopTaskCnt;                                           // 停止采集任务的次数(即调用AI_StopTask()的次数)
    U32 nTransRate;                                             // 传输速率, 即每秒传输点数(sps)，作为USB及应用软件传输性能的监测信息

    U32 nReserved0;                                             // 保留字段(暂未定义).
    U32 nReserved1;                                             // 保留字段(暂未定义)
    U32 nReserved2;                                             // 保留字段(暂未定义)
} ACTS2200_AI_STATUS, *PACTS2200_AI_STATUS;

// #################### AI主要信息结构体(ACTS2200_AI_MAIN_INFO) #######################
typedef struct _ACTS2200_AI_MAIN_INFO
{
    U32 nAIChannelCount;                                        // AI通道数量

    U32 nAISampRangeCount;                                      // AI采样范围挡位数量
    U32 nAISampRangeVal[ACTS2200_AI_MAX_RANGES];                // AI各采样范围对应档位值

    U32 nAISampSignalCount;                                     // AI采样信号数量
    U32 nAISampSignalVal[ACTS2200_AI_SAMPSIGNAL_COUNT];         // AI各采样信号对应值

    U32 nAISampGainCount;                                       // AI采样增益挡位数量(保留)
    U32 nAICouplingCount;                                       // AI耦合挡位数量(保留)
    U32 nAIImpedanceCount;                                      // AI阻抗的挡位数量(保留)
    U32 nAIDepthOfMemory;                                       // AI板载存储器深度(点数)
    U32 nAICodeType;                                            // AI码值类型(如=0表示原码; =1表示补码)
    U32 nAISampResolution;                                      // AI采样分辨率(如=8表示8Bit; =12表示12Bit; =14表示14Bit; =16表示16Bit)
    U32 nAISampCodeCount;                                       // AI采样编码数量(如256, 4096, 16384, 65536)
    U32 nAISupportAPFI;                                         // AI是否支持ATR
    U32 nAITrigLvlResolution;                                   // AI触发电平分辨率(如=8表示8Bit; =12表示12Bit; =16表示16Bit)
    U32 nAITrigLvlCodeCount;                                    // AI触发电平编码数量(如256, 4096)
    F64 fAIBaseRate;                                            // AI基准频率Hz
    F64 fAISingleChnMaxRate;                                    // AI单通道时最大频率Hz
    F64 fAIMultiChnMaxRate;                                     // AI多通道切换时最大频率Hz

    F64 fVREF_P;                                                // AI正基准电压
    F64 fVREF_N;                                                // AI负基准电压

    U32 nReserved0;                                             // 保留字段(暂未定义)
    U32 nReserved1;                                             // 保留字段(暂未定义)
    U32 nReserved2;                                             // 保留字段(暂未定义)
} ACTS2200_AI_MAIN_INFO, *PACTS2200_AI_MAIN_INFO;

// #################### AI采样范围信息结构体(ACTS2200_AI_VOLT_RANGE_INFO) #######################
typedef struct _ACTS2200_AI_VOLT_RANGE_INFO
{
    U32 nSampRange;                                             // 当前采样范围挡位号
    U32 nReserved0;                                             // 保留字段(暂未定义)
    F64 fMaxVolt;                                               // 采样范围的最大电压值,单位:伏(V)
    F64 fMinVolt;                                               // 采样范围的最小电压值,单位:伏(V)
    F64 fAmplitude;                                             // 采样范围幅度,单位:伏(V)
    F64 fHalfOfAmp;                                             // 采样范围幅度的二分之一,单位:伏(V)
    F64 fCodeWidth;                                             // 编码宽度,单位:伏(V), 即每个单位码值所分配的电压值
    F64 fOffsetVolt;                                            // 偏移电压,单位:伏(V),一般用于零偏校准
    F64 fOffsetCode;                                            // 偏移码值,一般用于零偏校准,它代表的电压值等价于fOffsetVolt
    char strDesc[16];                                           // 采样范围字符描述,如"±10V", "0-10V"等

    U32 nPolarity;                                              // 采样范围的极性(0=双极性BiPolar, 1=单极性UniPolar)
    U32 nCodeCount;                                             // 原码数量
    I32 nMaxCode;                                               // 原码最大值
    I32 nMinCode;                                               // 原码最小值

    U32 nReserved1;                                             // 保留字段(暂未定义)
    U32 nReserved2;                                             // 保留字段(暂未定义)
    U32 nReserved3;                                             // 保留字段(暂未定义)
    U32 nReserved4;                                             // 保留字段(暂未定义)
} ACTS2200_AI_VOLT_RANGE_INFO, *PACTS2200_AI_VOLT_RANGE_INFO;

// 结构体AI_VOLT_RANGE_INFO的数据成员Polarity所用的采样范围极性选项
#define ACTS2200_VAL_POLAR_BIPOLAR                   0          // 双极性
#define ACTS2200_VAL_POLAR_UNIPOLAR                  1          // 单极性

// #################### AI速率信息结构体(ACTS2200_AI_SAMP_RATE_INFO) #######################
typedef struct _ACTS2200_AI_SAMP_RATE_INFO
{
    F64 fMaxRate;                                               // 单通道最大采样速率(sps)，多通道时各通道平分最大采样率
    F64 fMinRate;                                               // 单通道最小采样速率(sps)，多通道时各通道平分最小采样率
    F64 fTimeBase;                                              // 时钟基准（即板载晶振频率）单位：Hz
    U32 nDivideMode;                                            // 分频模式，0=整数分频(INTDIV), 1=DDS分频(DDSDIV)
    U32 nRateType;                                              // 速率类型,指fMaxRate和fMinRate的类型, =0:表示为所有采样通道的总速率, =1:表示为每个采样通道的速率

    U32 nReserved0;                                             // 保留字段(暂未定义)
    U32 nReserved1;                                             // 保留字段(暂未定义)
} ACTS2200_AI_SAMP_RATE_INFO, *PACTS2200_AI_SAMP_RATE_INFO;


///////////////////// AO ////////////////////////
// ##################### AO工作参数结构体ACTS2200_AO_PARAM描述 ###################
typedef struct _ACTS2200_AO_CH_PARAM                            // AI通道参数结构体
{
    U32 bChannelEn;                                             // 通道采样允许
    U32 nSampRange;                                             // 采样范围，具体定义请参考下面常量定义
    U32 nReserved0;                                             // 保留(未用)
    U32 nReserved1;                                             // 保留(未用)
} ACTS2200_AO_CH_PARAM, *PACTS2200_AO_CH_PARAM;

// AO硬件参数结构体ACTS2200_AO_CH_PARAM中的nSampRange参数所使用的模拟量输入采样范围挡位选项
#define ACTS2200_AO_SAMPRANGE_N10_P10V              0           // ±10V
#define ACTS2200_AO_SAMPRANGE_COUNT                 1

typedef struct _ACTS2200_AO_START_TRIG                          // 开始触发参数
{
    U32 nTrigType;                                              // 触发类型
    U32 nTrigSrc;                                               // 触发源, 详见下面常量定义
    U32 nTrigDir;                                               // 触发方向
    U32 bReTrig;                                                // 重触发使能, 1:使能
    F32 fTrigLvlTop;                                            // 保留
    F32 fTrigLvlBtm;                                            // 保留
    F64 fTrigDigFltrMinPulseWidth;                              // 触发数字滤波最小脉冲宽度，单位秒(S)
    F64 fDelaySeconds;                                          // 触发延迟时间，单位秒(S)
    U32 nReserved2;                                             // 保留字段(暂未定义)
    U32 nReserved3;                                             // 保留字段(暂未定义)
} ACTS2200_AO_START_TRIG, *PACTS2200_AO_START_TRIG;

typedef struct _ACTS2200_AO_PAUSE_TRIG                          // 暂停触发参数
{
    U32 nTrigType;                                              // 触发类型
    U32 nTrigSrc;                                               // 触发源, 详见下面常量定义
    U32 nTrigDir;                                               // 触发方向
    U32 nReserved0;                                             // 保留字段(暂未定义)
    F32 fTrigLvlTop;                                            // 保留
    F32 fTrigLvlBtm;                                            // 保留
    F64 fTrigDigFltrMinPulseWidth;                              // 触发数字滤波最小脉冲宽度，单位秒(S)
    U32 nReserved3;                                             // 保留字段(暂未定义)
} ACTS2200_AO_PAUSE_TRIG, *PACTS2200_AO_PAUSE_TRIG;

// AO开始触发参数结构体ACTS2200_AO_START_TRIG中的nTrigType触发类型所使用的选项
#define ACTS2200_VAL_NONE                           0           // 无触发(等同于软件强制触发)
#define ACTS2200_VAL_ANALOG_EDGE                    1           // 模拟边沿触发类型(保留,未提供)
#define ACTS2200_VAL_ANALOG_WIN                     2           // 模拟窗触发类型(保留,未提供)
#define ACTS2200_VAL_DIGIT_EDGE                     3           // 数字边沿触发类型
#define ACTS2200_VAL_DIGIT_PATTERN                  4           // 数字模式触发类型(保留,未提供)

// AO开始触发参数结构体ACTS2200_AO_PAUSE_TRIG中的nTrigType触发类型所使用的选项
#define ACTS2200_VAL_NONE                           0           // 无触发(禁用暂停触发)
#define ACTS2200_VAL_ANALOG_LVL                     1           // 模拟电平发类型(保留,未提供)
#define ACTS2200_VAL_ANALOG_WIN                     2           // 模拟窗触发类型(保留,未提供)
#define ACTS2200_VAL_DIGIT_LVL                      3           // 数字电平触发类型
#define ACTS2200_VAL_DIGIT_PATTERN                  4           // 数字模式触发类型(保留,未提供)

// AO开始触发参数结构体ACTS2200_AO_START_TRIG中的nTrigSrc触发源所使用的选项
// AO开始触发参数结构体ACTS2200_AO_PAUSE_TRIG中的nTrigSrc触发源所使用的选项
#define ACTS2200_VAL_PFI0                           0x110       // 接线端PFI0
#define ACTS2200_VAL_PFI1                           0x111       // 接线端PFI1
#define ACTS2200_VAL_PFI2                           0x112       // 接线端PFI2
#define ACTS2200_VAL_PFI3                           0x113       // 接线端PFI3
#define ACTS2200_VAL_PFI4                           0x114       // 接线端PFI4
#define ACTS2200_VAL_PFI5                           0x115       // 接线端PFI5
#define ACTS2200_VAL_PFI6                           0x116       // 接线端PFI6
#define ACTS2200_VAL_PFI7                           0x117       // 接线端PFI7

// AO开始触发参数结构体ACTS2200_AO_START_TRIG中的nTrigDir触发方向所使用的选项
// 当触发类型为模拟边沿触发或数字边沿触发时nTrigType = TRIGTYPE_ANALOG_EDGE/TRIGTYPE_DIGIT_EDGE
#define ACTS2200_VAL_FALLING                        0           // 下降沿
#define ACTS2200_VAL_RISING                         1           // 上升沿
#define ACTS2200_VAL_CHANGING                       2           // 变化(即上下边沿均有效)

// 当触发类型为模拟窗触发(nTrigType = TRIGTYPE_ANALOG_WIN):
#define ACTS2200_VAL_EnteringWin                    0           // 入窗
#define ACTS2200_VAL_LeavingWin                     1           // 出窗
#define ACTS2200_VAL_LeavingEnterWin                2           // 变化(出入窗均有效)

// AO暂停触发参数结构体ACTS2200_AO_PAUSE_TRIG中的nTrigDir触发方向所使用的选项
// 当触发类型为模拟电平触发或数字电平触发时(nTrigType = TRIGTYPE_ANALOG_LVL/TRIGTYPE_DIGIT_LVL):
#define ACTS2200_VAL_LOW                            0           // 低电平
#define ACTS2200_VAL_HIGH                           1           // 高电平

// 当触发类型为模拟窗触发(nTrigType = TRIGTYPE_ANALOG_WIN):
#define ACTS2200_VAL_InsideWin                      0           // 窗内
#define ACTS2200_VAL_OutsideWin                     1           // 窗外

typedef struct _ACTS2200_AO_PARAM                               // 跟整个AO有关的硬件工作参数(简称AO工作参数)
{
    // 通道参数
    U32 nSampChanCount;                                         // 采样通道总数[1, 4](只读参数, 它的取值由各通道参数CHParam[n].bChannelEn决定)
    U32 nReserved0;                                             // 保留字段(暂未定义)
    ACTS2200_AO_CH_PARAM CHParam[4];                            // 通道参数（通道使能，采样范围）

    // 时钟参数
    U32 nSampMode;                                              // 采样模式, 0=软件定时单点采样(按需)， 1、硬件定时单点采样(暂不支持)，2=有限点采样， 3=连续采样
    U32 nSampsPerChan;                                          // 每个通道生成点数(也是每通道待写入点数),取值范围为单点采样模式时应等于1，有限点或连续采样时为[2, 1024*1024]
    F64 fSampRate;                                              // 采样速率(Sample Rate), 单位为sps，取值范围为[1sps, 1M sps],它是每个采样通道的采样速率
    U32 nSampClkSrc;                                            // 采样时钟源选择,详见以下常量定义
    U32 nExtSampClkEdge;                                        // 外部采样时钟边沿, 0=下降沿, 1=上升沿(仅当nSampClkSrc外部采样时钟源时有效)
    U32 bRegenModeEn;                                           // 波形重生成模式允许(在连续采样模式中有效), =TRUE:只是对开始生成任前写入任务中的波形段数据进行循环重复生成，=FALSE:禁止重生成模式，在开始生成任务后，还要不断的往任务中写入新的波形数据
    U32 nReserved1;                                             // 保留字段(暂未定义)

    ACTS2200_AO_START_TRIG StartTrig;                           // 开始触发参数
    ACTS2200_AO_PAUSE_TRIG PauseTrig;                           // 暂停触发参数

    // 其他参数
    U32 nReserved2;                                             // 保留字段(暂未定义)
    U32 nReserved3;                                             // 保留字段(暂未定义)
    U32 nReserved4;                                             // 保留字段(暂未定义)
    U32 nReserved5;                                             // 保留字段(暂未定义)
} ACTS2200_AO_PARAM, *PACTS2200_AO_PARAM;

// AO硬件参数结构体ACTS2200_AO_PARAM中的nSampClkSrc采样时钟源所使用的选项
#define ACTS2200_VAL_LOCAL                          0           // 使用本地源
#define ACTS2200_VAL_PFI0                           0x110       // 接线端PFI0
#define ACTS2200_VAL_PFI1                           0x111       // 接线端PFI1
#define ACTS2200_VAL_PFI2                           0x112       // 接线端PFI2
#define ACTS2200_VAL_PFI3                           0x113       // 接线端PFI3
#define ACTS2200_VAL_PFI4                           0x114       // 接线端PFI4
#define ACTS2200_VAL_PFI5                           0x115       // 接线端PFI5
#define ACTS2200_VAL_PFI6                           0x116       // 接线端PFI6
#define ACTS2200_VAL_PFI7                           0x117       // 接线端PFI7

// AO硬件参数结构体ACTS2200_AO_PARAM中的nExtSampClkEdge外部采样时钟边沿所使用的选项
#define ACTS2200_VAL_FALLING                         0          // 下降沿
#define ACTS2200_VAL_RISING                          1          // 上升沿

// #################### AO工作状态结构体ACTS2200_AO_STATUS描述 #####################
typedef struct _ACTS2200_AO_STATUS
{
    U32 bTaskDone;                                              // 生成任务完成标志, =TRUE:表示生成任务完成, =FALSE:表示生成任务未完成，正在进行中
    U32 bTriged;                                                // 触发标志, =TRUE:表示已被触发, =FALSE:表示未被触发(即正等待触发)

    U32 nTaskState;                                             // 生成任务状态, =1:正常, 其它值表示有异常情况
    U32 nAvailSampsPerChan;                                     // 可写点数，最好等它大于参数nWriteSampsPerChan时才能调用AO_WriteAnalog()写入采样数据
    U32 nMaxAvailSampsPerChan;                                  // 自AO_StartTask()后出现过的最大可写点数，状态值范围[0, nBufSampsPerChan],它是为监测采集软件性能而提供，如果此值越趋近于1，则表示意味着性能越高，越不易出现溢出丢点的可能
    U32 nBufSampsPerChan;                                       // 每通道缓冲区大小(采样点数)。
    I64 nSampsPerChanAcquired;                                  // 每通道已采样点数(自开始生成任务(AO_StartTask())之后所采样的点数)，这个只是给用户的统计数据

    U32 nHardUnderflowCnt;                                      // 硬件缓冲下溢次数(在不下溢情况下恒等于0)
    U32 nSoftUnderflowCnt;                                      // 软件缓冲下溢次数(在不下溢情况下恒等于0)
    U32 nInitTaskCnt;                                           // 初始化生成任务的次数(即调用AO_InitTask()的次数)
    U32 nReleaseTaskCnt;                                        // 释放生成任务的次数(即调用AO_ReleaseTask()的次数)
    U32 nStartTaskCnt;                                          // 开始生成任务的次数(即调用AO_StartTask()的次数)
    U32 nStopTaskCnt;                                           // 停止生成任务的次数(即调用AO_StopTask()的次数)
    U32 nTransRate;                                             // 传输速率, 即每秒传输点数(SPS)，作为USB及应用软件传输性能的监测信息

    U32 nReserved0;                                             // 保留字段(暂未定义)
    U32 nReserved1;                                             // 保留字段(暂未定义)
    U32 nReserved2;                                             // 保留字段(暂未定义)
} ACTS2200_AO_STATUS, *PACTS2200_AO_STATUS;

// #################### AO主要信息结构体(ACTS2200_AO_MAIN_INFO) #######################
typedef struct _ACTS2200_AO_MAIN_INFO
{
    U32 nChannelCount;                                          // AO通道数量o

    U32 nSampRangeCount;                                        // AO采样范围挡位数量
    U32 nAOSampRangeVal[ACTS2200_AO_SAMPRANGE_COUNT];           // AO各采样范围对应档位值

    U32 nSampGainCount;                                         // AO增益挡位数量(保留)
    U32 nCouplingCount;                                         // AO耦合挡位数量(保留)
    U32 nImpedanceCount;                                        // AO阻抗的挡位数量(保留)
    U32 nDepthOfMemory;                                         // AO板载存储器深度(点数)
    U32 nSampResolution;                                        // AO采样分辨率(如=8表示8Bit; =12表示12Bit; =14表示14Bit; =16表示16Bit)
    U32 nSampCodeCount;                                         // AO采样编码数量(如256, 4096, 16384, 65536)
    U32 nAOSupportAPFI;                                         // AO是否支持APFI(ATR触发下的方式,AO只支持APFI触发,如果AO不支持APFI即不支持ATR触发)
    U32 nTrigLvlResolution;                                     // 触发电平分辨率(如=8表示8Bit; =12表示12Bit; =16表示16Bit)
    U32 nTrigLvlCodeCount;                                      // 触发电平编码数量(如256, 4096)
    U32 nReserved0;                                             // 保留字段(暂未定义)
    F64 fAOBaseRate;                                            // AO基准频率Hz
    F64 fAOMaxRate;                                             // AO最大频率Hz

    U32 nReserved1;                                             // 保留字段(暂未定义)
    U32 nReserved2;                                             // 保留字段(暂未定义)
} ACTS2200_AO_MAIN_INFO, *PACTS2200_AO_MAIN_INFO;

// #################### AO采样范围信息结构体(ACTS2200_AO_VOLT_RANGE_INFO) #######################
typedef struct _ACTS2200_AO_VOLT_RANGE_INFO
{
    U32 nSampRange;                                             // 当前采样范围挡位号
    U32 nReserved0;                                             // 保留字段(暂未定义)
    F64 fMaxVolt;                                               // 采样范围的最大电压值,单位:伏(V)
    F64 fMinVolt;                                               // 采样范围的最小电压值,单位:伏(V)
    F64 fAmplitude;                                             // 采样范围幅度,单位:伏(V)
    F64 fHalfOfAmp;                                             // 采样范围幅度的二分之一,单位:伏(V)
    F64 fCodeWidth;                                             // 编码宽度,单位:伏(V), 即每个单位码值所分配的电压值
    F64 fOffsetVolt;                                            // 偏移电压,单位:伏(V),一般用于零偏校准
    F64 fOffsetCode;                                            // 偏移码值,一般用于零偏校准,它代表的电压值等价于fOffsetVolt
    char strDesc[16];                                           // 采样范围字符描述,如"±10V", "0-10V"等

    U32 nPolarity;                                              // 采样范围的极性(0=双极性BiPolar, 1=单极性UniPolar)
    U32 nCodeCount;                                             // 原码数量
    I32 nMaxCode;                                               // 原码最大值
    I32 nMinCode;                                               // 原码最小值

    U32 nReserved1;                                             // 保留字段(暂未定义)
    U32 nReserved2;                                             // 保留字段(暂未定义)
    U32 nReserved3;                                             // 保留字段(暂未定义)
    U32 nReserved4;                                             // 保留字段(暂未定义)
} ACTS2200_AO_VOLT_RANGE_INFO, *PACTS2200_AO_VOLT_RANGE_INFO;

// 结构体AO_VOLT_RANGE_INFO的数据成员Polarity所用的采样范围极性选项
#define ACTS2200_VAL_POLAR_BIPOLAR                   0          // 双极性
#define ACTS2200_VAL_POLAR_UNIPOLAR                  1          // 单极性

// #################### AO速率信息结构体(ACTS2200_AO_SAMP_RATE_INFO) #######################
typedef struct _ACTS2200_AO_SAMP_RATE_INFO
{
    F64 fMaxRate;                                               // 单通道最大采样速率(sps)，多通道时各通道平分最大采样率
    F64 fMinRate;                                               // 单通道最小采样速率(sps)，多通道时各通道平分最小采样率
    F64 fTimeBase;                                              // 时钟基准（即板载晶振频率）单位：Hz
    U32 nDivideMode;                                            // 分频模式，0=整数分频(INTDIV), 1=DDS分频(DDSDIV)

    U32 nRateType;                                              // 速率类型,指fMaxRate和fMinRate的类型, =0:表示为所有采样通道的总速率, =1:表示为每个采样通道的速率
    U32 nReserved0;                                             // 保留字段(暂未定义)
    U32 nReserved1;                                             // 保留字段(暂未定义)
} ACTS2200_AO_SAMP_RATE_INFO, *PACTS2200_AO_SAMP_RATE_INFO;

///////////////////// DI/O ////////////////////////
#define ACTS2200_DIO_PORT0_MAX_LINES                8           // 数字量端口0支持8条线输入\输出,方向可控,Port0只能为DIO
#define ACTS2200_DIO_PORT1_MAX_LINES                8           // 数字量端口1支持8条线输入\输出,方向可控,PFI0~7
#define ACTS2200_DIO_PORT2_MAX_LINES                8           // 数字量端口2支持8条线输入\输出,方向可控,PFI8~15

// 函数DIO_SetPowerOnState()的参数nState所用的状态类型
// 函数DIO_GetPowerOnState()的参数nState所用的状态类型
#define ACTS2200_DIO_POWERUPSTATE_DI			    0           // 上电配置为输入
#define ACTS2200_DIO_POWERUPSTATE_LOW		        1           // 上电配置为输出低
#define ACTS2200_DIO_POWERUPSTATE_HIGH		        2           // 上电配置为输出高


//*** Value set ChannelType ***
#define ArtDAQ_Val_CI                               10131       // Counter Input
#define ArtDAQ_Val_CO                               10132       // Counter Output

//*** Value set CIMeasurementType ***
#define ArtDAQ_Val_Freq                             10179       // Frequency
#define ArtDAQ_Val_Period                           10256       // Period
#define ArtDAQ_Val_CountEdges                       10125       // Count Edges
#define ArtDAQ_Val_PulseWidth                       10359       // Pulse Width
#define ArtDAQ_Val_SemiPeriod                       10289       // Semi Period
#define ArtDAQ_Val_PulseFrequency                   15864       // Pulse Frequency
#define ArtDAQ_Val_PulseTime                        15865       // Pulse Time
#define ArtDAQ_Val_PulseTicks                       15866       // Pulse Ticks
#define ArtDAQ_Val_Position_AngEncoder              10360       // Position:Angular Encoder
#define ArtDAQ_Val_Position_LinEncoder              10361       // Position:Linear Encoder
#define ArtDAQ_Val_TwoEdgeSep                       10267       // Two Edge Separation

//*** Value set COOutputType ***
#define ArtDAQ_Val_Pulse_Time                       10269       // Pulse:Time
#define ArtDAQ_Val_Pulse_Freq                       10119       // Pulse:Frequency
#define ArtDAQ_Val_Pulse_Ticks                      10268       // Pulse:Ticks

//*** Values for CTR_CfgDigEdgeStartTrig.nTrigSrc ***
//*** Values for CTR_CfgDigLvlPauseTrig.nTrigSrc ***
//*** Values for CI_SetCountEdgesCountResetParam.nResetTerm ***
//*** Value set TermSrc ***
#define ACTS2200_VAL_PFI0                           0x110       // 接线端PFI0
#define ACTS2200_VAL_PFI1                           0x111       // 接线端PFI1
#define ACTS2200_VAL_PFI2                           0x112       // 接线端PFI2
#define ACTS2200_VAL_PFI3                           0x113       // 接线端PFI3
#define ACTS2200_VAL_PFI4                           0x114       // 接线端PFI4
#define ACTS2200_VAL_PFI5                           0x115       // 接线端PFI5
#define ACTS2200_VAL_PFI6                           0x116       // 接线端PFI6
#define ACTS2200_VAL_PFI7                           0x117       // 接线端PFI7

//*** Values for CI_CreatePeriodChan.nUnits 	***
//*** Values for CI_CreatePulseWidthChan.nUnits ***
//*** Values for CI_CreateTwoEdgeSepChan.nUnits ***
//*** Values for CI_CreateSemiPeriodChan.nUnits ***
//*** Values for CI_SetDigitalFilter.nUnits 	***
//*** Value set TimeUnits3 ***
#define ArtDAQ_Val_Seconds                          10364       // Seconds
#define ArtDAQ_Val_Ticks                            10304       // Ticks

//*** Values for CI_CreateFreqChan.nUnits ***
//*** Value set FrequencyUnits3 ***
#define ArtDAQ_Val_Hz                               10373       // Hz
#define ArtDAQ_Val_Ticks                            10304       // Ticks

//*** Values for CI_CreatePulseChanFreq.nUnits ***
//*** Values for CO_CreatePulseChanFreq.nUnits ***
//*** Value set FrequencyUnits2 ***
#define ArtDAQ_Val_Hz                               10373       // Hz

//*** Values for CI_CreatePulseChanTime.nUnits ***
//*** Values for CO_CreatePulseChanTime.nUnits ***
//*** Value set TimeUnits2 ***
#define ArtDAQ_Val_Seconds                          10364       // Seconds

//*** Values for CI_CreateAngEncoderChan.nUnits ***
//*** Value set AngleUnits2 ***
#define ArtDAQ_Val_Degrees                          10146       // Degrees
#define ArtDAQ_Val_Radians                          10273       // Radians
#define ArtDAQ_Val_Ticks                            10304       // Ticks

//*** Values for CI_CreateLinEncoderChan.nUnits ***
//*** Value set LengthUnits3 ***
#define ArtDAQ_Val_Meters                           10219       // Meters
#define ArtDAQ_Val_Inches                           10379       // Inches
#define ArtDAQ_Val_Ticks                            10304       // Ticks

//*** Values for CI_CreateFreqChan.nMeasMethod 		***
//*** Values for CI_CreatePeriodChan.nMeasMethod 	***
//*** Value set CounterFrequencyMethod ***
#define ArtDAQ_Val_LowFreq1Ctr                      10105       // Low Frequency with 1 Counter
#define ArtDAQ_Val_HighFreq2Ctr                     10157       // High Frequency with 2 Counters
#define ArtDAQ_Val_LargeRng2Ctr                     10205       // Large Range with 2 Counters

//*** Values for CI_CreateCountEdgesChan.nCountDirection ***
//*** Value set CountDirection1 ***
#define ArtDAQ_Val_CountUp                          10128       // Count Up
#define ArtDAQ_Val_CountDown                        10124       // Count Down
#define ArtDAQ_Val_ExtControlled                    10326       // Externally Controlled

//*** Values for CI_CreateLinEncoderChan.nDecodingType ***
//*** Values for CI_CreateAngEncoderChan.nDecodingType ***
//*** Value set EncoderType2 ***
#define ArtDAQ_Val_X1                               10090       // X1
#define ArtDAQ_Val_X2                               10091       // X2
#define ArtDAQ_Val_X4                               10092       // X4
#define ArtDAQ_Val_TwoPulseCounting                 10313       // Two Pulse Counting

//*** Values for CI_CreateLinEncoderChan.nZidxPhase ***
//*** Values for CI_CreateAngEncoderChan.nZidxPhase ***
//*** Value set EncoderZIndexPhase1 ***
#define ArtDAQ_Val_AHighBHigh                       10040       // A High B High
#define ArtDAQ_Val_AHighBLow                        10041       // A High B Low
#define ArtDAQ_Val_ALowBHigh                        10042       // A Low B High
#define ArtDAQ_Val_ALowBLow                         10043       // A Low B Low

//*** Values for CI_CfgOutputEvent.nBehavior ***
//*** Value set ExportActions2 ***
#define ArtDAQ_Val_Pulse                            10265       // Pulse
#define ArtDAQ_Val_Toggle                           10307       // Toggle

//*** Values for CI_CfgOutputEvent.nPolarity ***
//*** Value set Polarity2 ***
#define ArtDAQ_Val_ActiveHigh                       10095       // Active High
#define ArtDAQ_Val_ActiveLow                        10096       // Active Low

//*** Values for CI_CfgOutputEvent.nIdleState 		***
//*** Values for CO_GetOutputState.pOutputState 	***
//*** Values for CO_CreatePulseChanFreq.nIdleState 	***
//*** Values for CO_CreatePulseChanTime.nIdleState 	***
//*** Values for CO_CreatePulseChanTicks.nIdleState ***
//*** Values for CTR_CfgDigLvlPauseTrig.nTrigWhen 	***
//*** Value set Level1 ***
#define ArtDAQ_Val_High                             10192       // High
#define ArtDAQ_Val_Low                              10214       // Low

//*** Values for CI_CreateFreqChan.nStartingEdge ***
//*** Values for CI_CreatePeriodChan.nStartingEdge ***
//*** Values for CI_CreateCountEdgesChan.nActiveEdge ***
//*** Values for CI_SetCountEdgesCountResetParam.nActiveEdge ***
//*** Values for CI_CreatePulseWidthChan.nStartingEdge ***
//*** Values for CI_CreateTwoEdgeSepChan.nFirstEdge ***
//*** Values for CI_CreateTwoEdgeSepChan.nSecondEdge ***
//*** Values for CTR_CfgDigEdgeStartTrig.nTrigEdge ***
//*** Value set Edge1 ***
#define ArtDAQ_Val_Rising                           10280       // Rising
#define ArtDAQ_Val_Falling                          10171       // Falling

//*** Values for CI_SetDigitalFilter.nSignal ***
// Values for CTR Digital filter
#define ArtDAQ_Val_Src                              20101
#define ArtDAQ_Val_Gate                             20102
#define ArtDAQ_Val_Aux                              20103

//*** Values for CI_CreatePulseChanTicks.nSrcTerminal ***
//*** Values for CO_CreatePulseChanTicks.nSrcTerminal ***
// Values for CTR TimeBase
#define ACTS2200_VAL_100MHz                         0           // 内部100MHz时基
#define ACTS2200_VAL_20MHz                          1           // 内部20MHz时基(保留)
#define ACTS2200_VAL_10MHz                          2           // 内部10MHz时基(保留)
#define ACTS2200_VAL_100KHz                         3           // 内部100KHz时基
#define ACTS2200_VAL_PFI0                           0x110       // 接线端PFI0
#define ACTS2200_VAL_PFI1                           0x111       // 接线端PFI1
#define ACTS2200_VAL_PFI2                           0x112       // 接线端PFI2
#define ACTS2200_VAL_PFI3                           0x113       // 接线端PFI3
#define ACTS2200_VAL_PFI4                           0x114       // 接线端PFI4
#define ACTS2200_VAL_PFI5                           0x115       // 接线端PFI5
#define ACTS2200_VAL_PFI6                           0x116       // 接线端PFI6
#define ACTS2200_VAL_PFI7                           0x117       // 接线端PFI7

// #################### AI、AO、DI、DO、CI、CO函数共用的参数常量定义 #######################
// #################### 主要信息结构体(ACTS2200_MAIN_INFO) #######################
typedef struct _ACTS2200_MAIN_INFO
{
    ACTS2200_AI_MAIN_INFO AIInfo;                               // AI信息
    ACTS2200_AO_MAIN_INFO AOInfo;                               // AO信息

    U32 nDIOPortCount;                                          // DIO端口数量
    U32 nDIOPortLines[ACTS2200_DIO_MAX_PORTS];                  // DIO各Port的线数量
    U32 nCTRChannelCount;                                       // CTR通道数量
    U32 nCIChannelCount;                                        // CI通道数量
    U32 nCOChannelCount;                                        // CO通道数量

    U32 b5VPowerSupported;                                      // 本卡是否支持5V电源输出   1：支持；0：不支持。
    U32 nPFILines;                                              // 本卡支持的PFI数量

    U32 nReserved1;                                             // 保留字段(暂未定义)
    U32 nReserved2;                                             // 保留字段(暂未定义)
} ACTS2200_MAIN_INFO, *PACTS2200_MAIN_INFO;

// 硬件参数结构体中的nSampMode采样模式所使用的选项
#define ACTS2200_SAMPMODE_ONE_DEMAND                0           // 软件按需单点采样
#define ACTS2200_SAMPMODE_FINITE                    2           // 有限点采样
#define ACTS2200_SAMPMODE_CONTINUOUS                3           // 连续采样

// 函数AI_ReadAnalog()/AI_ReadBinary()、AO_WriteAnalog()/AO_WriteBinary()、DI_ReadDigitalLines()/DO_WriteDigitalLines()的参数nFillMode所使用的填充方式选项
#define ACTS2200_FILLMODE_GroupByScanNumber         0           // 按扫描序号分组填充缓冲区(交叉)
#define ACTS2200_FILLMODE_GroupByChannel            1           // 按通道分组填充缓冲区(不交叉)

// ################################ 函数错误信息 ################################
#define ERROR_NO_AVAILABLE_SAMPS        (0xE0000000+1)          // 无有效点数
#define ERROR_TASK_FAIL                 (0xE0000000+2)          // 任务失败
#define ERROR_RATE_OVER_RANGE           (0xE0000000+3)          // 采样速率超限

// 函数FILE_Create()的参数nOptMode所用的文件操作方式(支持"或"指令实现多种方式并行操作)
#define ACTS2200_FILE_OPTMODE_CREATE_NEW            1           // 创建文件,如果文件存在则会出错
#define ACTS2200_FILE_OPTMODE_CREATE_ALWAYS         2           // 不管文件是否存在，总是要被创建(即可能改写前一个文件)
#define ACTS2200_FILE_OPTMODE_OPEN_EXISTING         3           // 打开必须已经存在的文件
#define ACTS2200_FILE_OPTMODE_OPEN_ALWAYS           4           // 打开文件，若该文件不在，则创建它

// 函数FILE_SetOffset()的参数nBaseMode所用的文件指针移动参考基点
#define ACTS2200_FILE_BASEMODE_BEGIN                0           // 以文件起点作为参考点往右偏移
#define ACTS2200_FILE_BASEMODE_CURRENT              1           // 以文件的当前位置作为参考点往左或往右偏移(nOffsetBytes<0时往左偏移，>0时往右偏移)
#define ACTS2200_FILE_BASEMODE_END                  2           // 以文件的尾部作为参考点往左偏移

// 获取设备列表使用结构体
typedef struct _ACTS2200_DEV_INFO
{
    U32     nSerialCode;                                        // 设备的序列号
    U32     nDeviceType;                                        // 识别设备类别 返回类似为0x1E428811或0x30638814
    BOOL    bUsed;                                              // 设备是否已被使用
}ACTS2200_DEV_INFO, *PACTS2200_DEV_INFO;

//#ifdef __cplusplus
//extern "C" {
//#endif
    // ################################ DEV设备对象管理函数 ################################
    BOOL  ACTS2200_DeviceList(                            // 枚举设备信息
        ACTS2200_DEV_INFO           devInfo[],                  // 设备信息
        U8                          nArraySize,                 // 设备信息数组大小
        U8*                         pDeviceNum);                // 返回的实际设备数

    HANDLE  ACTS2200_DEV_Create(                          // 打开设备并返回设备句柄
        U32                         nSerialNumber);             // 设备硬件系列号

    void  ACTS2200_DEV_Release(                           // 关闭设备
        HANDLE                      hDevice);                   // 设备对象句柄,它由DEV_Create()函数创建

    BOOL  ACTS2200_DEV_Reset(                             //  复位设备
        HANDLE                      hDevice);                   // 设备对象句柄,它由DEV_Create()函数创建

    BOOL  ACTS2200_DEV_GetSerialNumber(                   // 得到设备系列号
        HANDLE                      hDevice,                    // 设备对象句柄,它由DEV_Create()函数创建
        U32*                        pSerialNumber);             // 序列号

    BOOL  ACTS2200_DEV_GetVersion(                        // 获得设备版本信息, 成功时返回TRUE,否则返回FALSE
        HANDLE                      hDevice,                    // 设备对象句柄,它由DEV_Create()函数创建
        U32*                        pDllVer,                    // 返回的动态库(.dll)版本号
        U32*                        pDriverVer,                 // 返回的驱动(.sys)版本号
        U32*                        pFirmwareVer);              // 返回的固件版本号

    BOOL  ACTS2200_DEV_GetMainInfo(                       // 获得AI的主要信息
        HANDLE                      hDevice,                    // 设备对象句柄,它由DEV_Create()函数创建
        PACTS2200_MAIN_INFO         pMainInfo);                 // 获得设备主要信息

    // ================================ 设备辅助操作 ================================
    BOOL  ACTS2200_DEV_GetCalDate(
        HANDLE                      hDevice,                    // 设备对象句柄,它由DEV_Create()函数创建
        U64*                        pCalDate);                  // 返回最后校准日期

    BOOL  ACTS2200_DEV_GetFLHSerialNum(                   // 得到EEP存储的设备序列号
        HANDLE                      hDevice,                    // 设备对象句柄,它由DEV_Create()函数创建
        U32*                        pSerialNum);                // 返回设备的序列号

    BOOL  ACTS2200_DEV_ConfigSerialNum(                   // 配置设备序列号
        HANDLE                      hDevice,                    // 设备对象句柄,它由DEV_Create()函数创建
        U32                         nKey,                       // 输入对应密码，密码正确后才可以进行配置
        U32                         nSerialNum);                // 设备序列号

    // 以下为5V电源输出功能操作函数，卡是否支持5V电源输出功能可参考说明书，或者获取ACTS2200_MAIN_INFO，查看b5VPowerSupported得知
    BOOL  ACTS2200_DEV_GetPowerMode(			        	// 取得设备的电源模式(Get Power Mode), 成功时返回TRUE,否则返回FALSE
        HANDLE                      hDevice,		        	// 设备对象句柄,它由DEV_Create()函数创建
        U32*                        pPowerMode);	        	// 电源模式, =0:表示设备由USB总线供电, =FALSE:表示设备由外部电源供电(5)

    BOOL  ACTS2200_DEV_SetPower5VState(		        	// 设置+5V电源状态(Set +5V Power State), 成功时返回TRUE,否则返回FALSE
        HANDLE                      hDevice,		        	// 设备对象句柄,它由DEV_Create()函数创建
        BOOL                        bEnable);		        	// 允许输出, =TRUE:表示允许输出, =FALSE:表示禁止输出

    BOOL  ACTS2200_DEV_GetPower5VState(		        	// 允许+5V电源输出(Enable +5V Power Output), 成功时返回TRUE,否则返回FALSE
        HANDLE                      hDevice,		        	// 设备对象句柄,它由DEV_Create()函数创建
        BOOL*                       pEnabled,		        	// 是否已允许输出, =TRUE:表示已允许输出, =FALSE:表示已禁止输出
        BOOL*                       pOverload);		            // 是否过载, =TRUE:表示过载, =FALSE:表示未过载（正常）

    BOOL  ACTS2200_DEV_Set5VPowerOnState(		        	// 设置+5V电源上电状态(Set +5V Power State), 成功时返回TRUE,否则返回FALSE
        HANDLE                      hDevice,		        	// 设备对象句柄,它由DEV_Create()函数创建
        BOOL                        bEnable);		        	// 允许输出, =TRUE:表示允许输出, =FALSE:表示禁止输出

    BOOL  ACTS2200_DEV_Get5VPowerOnState(		        	// 设置+5V电源上电状态(Set +5V Power State), 成功时返回TRUE,否则返回FALSE
        HANDLE                      hDevice,		        	// 设备对象句柄,它由DEV_Create()函数创建
        BOOL*                       pEnable);		        	// 允许输出, =TRUE:表示允许输出, =FALSE:表示禁止输出


    // ################################ 信号导出实现函数 ################################
    BOOL  ACTS2200_ExportSignal(                          // 导出nSignalID指定的信号到nTerm指定的接线端
        HANDLE                      hDevice,                    // 设备对象句柄,它由DEV_Create()函数创建
        U32                         nSignalID,                  // 导出信号，包括采样时钟，触发等信号，详见上述nSignalID常量定义
        U32                         nTerm);                     // 信号导出的接线端，详见上述nTerm定义


    // ################################ AI模拟量输入实现函数 ################################
    BOOL  ACTS2200_AI_InitTask(                           // 初始化采集任务(Initialize Task)
        HANDLE                      hDevice,                    // 设备对象句柄,它由DEV_Create()函数创建
        PACTS2200_AI_PARAM          pAIParam);                  // 返回采样事件对象句柄,当设备中出现可读数据段时会触发此事件，参数=NULL,表示不需要此事件句柄

    BOOL  ACTS2200_AI_StartTask(                          // 开始采集任务
        HANDLE                      hDevice);                   // 设备对象句柄,它由DEV_Create()函数创建

    BOOL  ACTS2200_AI_SendSoftTrig(                       // 发送软件触发事件(Send Software Trig),软件触发也叫强制触发
        HANDLE                      hDevice);                   // 设备对象句柄,它由DEV_Create()函数创建

    BOOL  ACTS2200_AI_GetStatus(                          // 获取AI状态
        HANDLE                      hDevice,                    // 设备对象句柄,它由DEV_Create()函数创建
        PACTS2200_AI_STATUS         pAIStatus);                 // 取得AI各种状态

    BOOL  ACTS2200_AI_WaitUntilTaskDone(                  // 采集任务结束前等待,返回TRUE表示采集任务结束
        HANDLE                      hDevice,                    // 设备对象句柄,它由DEV_Create()函数创建
        F64                         fTimeout);                  // 用于等待的时间，单位：秒(S)

    BOOL  ACTS2200_AI_ReadAnalog(                         // 从采集任务中读取采样数据(电压数据序列)(Read analog data from the task)
        HANDLE                      hDevice,                    // 设备对象句柄,它由DEV_Create()函数创建
        F64                         fAnlgArray[],               // 模拟数据数组(电压数组),用于返回采样的电压数据，取值区间由各通道采样时的采样范围决定(单位:V)
        U32                         nReadSampsPerChan,          // 每通道请求读取的点数(单位：点)
        U32*                        pSampsPerChanRead,          // 返回每通道实际读取的点数(单位：点), =NULL,表示无须返回
        F64                         fTimeout,                   // 超时时间，单位：秒(S)
        U32                         nFillMode);                 // 缓冲填充方式, 0=GroupByScanNumber(按扫描通道序号分组，交叉), 1=GroupByChannel(按通道分组，不交叉)

    BOOL  ACTS2200_AI_ReadBinaryI16(                      // 从采集任务中读取采样数据(原码数据序列)(Read binary data from the task)
        HANDLE                      hDevice,                    // 设备对象句柄,它由DEV_Create()函数创建
        I16                         nBinArray[],                // 二进制数据数组（原码数组）,用于返回采样的原码数据,最高支持16位ADC
        U32                         nReadSampsPerChan,          // 每通道请求读取的点数(单位：点)
        U32*                        pSampsPerChanRead,          // 返回每通道实际读取的点数(单位：点), =NULL,表示无须返回
        F64                         fTimeout,                   // 超时时间，单位：秒(S)
        U32                         nFillMode);                 // 缓冲填充方式, 0=GroupByScanNumber(按扫描通道序号分组，交叉), 1=GroupByChannel(按通道分组，不交叉)

    BOOL  ACTS2200_AI_ReadBinaryI32(                      // 从采集任务中读取采样数据(原码数据序列)(Read binary data from the task)
        HANDLE                      hDevice,                    // 设备对象句柄,它由DEV_Create()函数创建
        I32                         nBinArray[],                // 二进制数据数组（原码数组）,用于返回采样的原码数据,最高支持32位ADC
        U32                         nReadSampsPerChan,          // 每通道请求读取的点数(单位：点)
        U32*                        pSampsPerChanRead,          // 返回每通道实际读取的点数(单位：点), =NULL,表示无须返回
        F64                         fTimeout,                   // 超时时间，单位：秒(S)
        U32                         nFillMode);                 // 缓冲填充方式, 0=GroupByScanNumber(按扫描通道序号分组，交叉), 1=GroupByChannel(按通道分组，不交叉)

    BOOL  ACTS2200_AI_StopTask(
        HANDLE                      hDevice);                   // 停止AI采集任务

    BOOL  ACTS2200_AI_ReleaseTask(
        HANDLE                      hDevice);                   // 释放采集任务

    // ================================ AI辅助操作函数 ================================
    BOOL  ACTS2200_AI_IsCaled(                            // AI是否已被自我校准过, 如果已校准过，则返回TRUE,否则返回FALSE
        HANDLE                      hDevice,                    // 设备对象句柄,它由DEV_Create()函数创建
        BOOL*                       pCaled);                    // 是否已经被校准，=TRUE:表示已经被校准过, =FALSE:表示未被校准过

    BOOL  ACTS2200_AI_SelfCal(                            // AI自我校准, 成功时返回TRUE,否则返回FALSE,可调用GetLastError()分析错误原因
        HANDLE                      hDevice);                   // 设备对象句柄,它由DEV_Create()函数创建

    BOOL  ACTS2200_AI_GetMainInfo(                        // 获得AI的主要信息
        HANDLE                      hDevice,                    // 设备对象句柄,它由DEV_Create()函数创建
        PACTS2200_AI_MAIN_INFO      pMainInfo);                 // 获得AI主要信息

    BOOL  ACTS2200_AI_GetVoltRangeInfo(                   // 获得采样范围的各种信息(幅度、分辨率、极性)
        HANDLE                      hDevice,                    // 设备对象句柄,它由DEV_Create()函数创建
        U32                         nChannel,                   // AI物理通道号[0, 3]
        U32                         nSampRange,                 // 采样范围选择[0, 3]
        PACTS2200_AI_VOLT_RANGE_INFO pRangeInfo);               // 采样范围信息

    BOOL  ACTS2200_AI_GetRateInfo(                        // 获得采样速率信息
        HANDLE                      hDevice,                    // 设备对象句柄,它由DEV_Create()函数创建
        PACTS2200_AI_SAMP_RATE_INFO pRateInfo);                 // 采样速率信息

    BOOL  ACTS2200_AI_ScaleBinToVolt(                     // 根据指定采样范围将采样原码数据量化成电压数据,返回实际量化的点数
        PACTS2200_AI_VOLT_RANGE_INFO pRangeInfo,                // 当前转换数据需要的采样范围信息
        PVOID                       pGainInfo,                  // 当前转换数据需要的采样增益信息(若于=NULL，表示不使用增益)
        F64                         fVoltArray[],               // 电压数据数组,用于返回量化后的电压数据,单位V
        I16                         nBinArray[],                // 二进制原码数组,用于传入量化前的原码数据，取值区间为[-8388608, 8388607], (单指某个通道的连续数据)
        U32                         nScaleSamps,                // 请求量化的采样点数
        U32*                        pSampsScaled);              // 返回实际量化的采样点数, =NULL,表示无须返回

    BOOL  ACTS2200_AI_ScaleVoltToBin(                     // 根据指定采样范围将电压数据量化成二进制原码数据,返回实际量化的点数
        PACTS2200_AI_VOLT_RANGE_INFO pRangeInfo,                // 当前转换数据需要的采样范围信息
        PVOID                       pGainInfo,                  // 当前转换数据需要的采样增益信息(若于=NULL，表示不使用增益)
        I16                         nBinArray[],                // 原码缓冲区,用于返回量化后的原码数据，取值区间为[-8388608, 8388607], (单指某个通道的连续数据)
        F64                         fVoltArray[],               // 电压缓冲区,用于传入量化前的电压数据,单位V
        U32                         nScaleSamps,                // 请求量化的采样点数
        U32*                        pSampsScaled);              // 返回实际量化的采样点数, =NULL,表示无须返回

    BOOL  ACTS2200_AI_VerifyParam(                        // 校验AI工作参数(Verify Parameter),建议在初始化AI参数前调用此函数校验各参数合法性
        HANDLE                      hDevice,                    // 设备对象句柄,它由DEV_Create()函数创建
        PACTS2200_AI_PARAM          pAIParam);                  // 待校验的AI工作参数

    BOOL  ACTS2200_AI_LoadParam(                          // 从ACTS2200.ini中加载AI参数
        HANDLE                      hDevice,
        PACTS2200_AI_PARAM          pAIParam);

    BOOL  ACTS2200_AI_SaveParam(                          // 保存AI参数至ACTS2200.ini
        HANDLE                      hDevice,
        PACTS2200_AI_PARAM          pAIParam);

    BOOL  ACTS2200_AI_ResetParam(                         // 将当前AI参数复位至出厂值
        HANDLE                      hDevice,
        PACTS2200_AI_PARAM          pAIParam);

    // ################################ AO模拟量输出实现函数 ################################
    BOOL  ACTS2200_AO_InitTask(			                // 初始化生成任务(Initialize Task)
        HANDLE                      hDevice,		        	// 设备对象句柄,它由DEV_Create()函数创建
        PACTS2200_AO_PARAM          pAOParam);                  // AO工作参数, 它仅在此函数中决定硬件初始状态和各工作模式,可以事先由AI_VerifyParam()进行参数校验

    BOOL  ACTS2200_AO_StartTask(			                // 开始生成任务(Start Task)
        HANDLE                      hDevice);		            // 设备对象句柄,它由DEV_Create()函数创建

    BOOL  ACTS2200_AO_SendSoftTrig(		                // 发送软件触发事件(Send Software Trig),软件触发也叫强制触发
        HANDLE                      hDevice);		            // 设备对象句柄,它由DEV_Create()函数创建

    BOOL  ACTS2200_AO_GetStatus(                          // 取得AO各种状态
        HANDLE                      hDevice,
        PACTS2200_AO_STATUS         pAOStatus);

    BOOL  ACTS2200_AO_WaitUntilTaskDone(	                // 生成任务结束前等待,返回TRUE表示生成任务结束
        HANDLE                      hDevice,		        	// 设备对象句柄,它由DEV_Create()函数创建
        F64                         fTimeout);		        	// 超时时间,单位:秒(S)

    BOOL  ACTS2200_AO_WriteAnalog(			            // 向生成任务中写入AO输出的模拟量电压数据(Write analog data to the task)
        HANDLE                      hDevice,		        	// 设备对象句柄,它由DEV_Create()函数创建
        F64                         fAnlgArray[],	        	// AO0、AO1电压混合缓冲区,存放AO的模拟(电压)数据,取值区间由各通道采样时的采样范围决定(单位:V)
        U32                         nWriteSampsPerChan,	        // 每通道请求写入的点数(单位:点)
        U32*                        pSampsPerChanWritten,       // 返回每通道实际写入的点数(单位:点), =NULL,表示无须返回
        F64                         fTimeout,                   // 超时时间，单位：秒(S)
        U32                         nFillMode);                 // 缓冲填充方式, 0=GroupByScanNumber(按扫描通道序号分组，交叉), 1=GroupByChannel(按通道分组，不交叉)

    BOOL  ACTS2200_AO_WriteBinary(			            // 向生成任务中写入AO输出的二进制原码数据(Write binary data to the task)
        HANDLE                      hDevice,			        // 设备对象句柄,它由DEV_Create()函数创建
        I16                         nBinArray[],		        // AO0、AO1原码混合缓冲区,用于输出生成的原码数据,取值区间为[0, 4095]
        U32                         nWriteSampsPerChan,	        // 每通道请求写入的点数(单位:点), =NULL,表示无须返回
        U32*                        pSampsPerChanWritten,       // 返回每通道实际写入的点数(单位:点), =NULL,表示无须返回
        F64                         fTimeout,                   // 超时时间，单位：秒(S)
        U32                         nFillMode);                 // 缓冲填充方式, 0=GroupByScanNumber(按扫描通道序号分组，交叉), 1=GroupByChannel(按通道分组，不交叉)

    BOOL  ACTS2200_AO_ReadbackAnalog(		                // 回读所有AO通道的当前生成的模拟量数据(电压数据序列)(Read back analog data from the task)
        HANDLE                      hDevice,			        // 设备对象句柄,它由DEV_Create()函数创建
        F64                         fAnlgArray[ACTS2200_AO_MAX_CHANNELS]);		// fAnlgArray[0]=AO0电压数据,fAnlgArray[1]=AO1电压数据, 取值区间由相应通道的采样范围决定

    BOOL  ACTS2200_AO_ReadbackBinary(		                // 回读所有AO通道的当前生成的模拟量数据(二进制原码数据序列)(Read back binary data from the task)
        HANDLE                      hDevice,			        // 设备对象句柄,它由DEV_Create()函数创建
        I16                         nBinArray[ACTS2200_AO_MAX_CHANNELS]);		// nBinArray[0]=AO0原码数据,nBinArray[1]=AO1原码数据, 取值区间[0, 4095]

    BOOL  ACTS2200_AO_StopTask(                           // 停止生成任务
        HANDLE                      hDevice);

    BOOL  ACTS2200_AO_ReleaseTask(                        // 释放生成任务
        HANDLE                      hDevice);

    // ================================ AO辅助操作函数 ================================
    BOOL  ACTS2200_AO_IsCaled(                            // AO是否已被自我校准过, 如果已校准过，则返回TRUE,否则返回FALSE
        HANDLE                      hDevice,                    // 设备对象句柄,它由DEV_Create()函数创建
        BOOL*                       pCaled);                    // 是否已经被校准，=TRUE:表示已经被校准过, =FALSE:表示未被校准过

    BOOL  ACTS2200_AO_SelfCal(                            // AO自我校准, 成功时返回TRUE,否则返回FALSE,可调用GetLastError()分析错误原因
        HANDLE                      hDevice);                   // 设备对象句柄,它由DEV_Create()函数创建

    BOOL  ACTS2200_AO_GetMainInfo(                        // 获得AI的主要信息
        HANDLE                      hDevice,                    // 设备对象句柄,它由DEV_Create()函数创建
        PACTS2200_AO_MAIN_INFO      pMainInfo);                 // 获得AO主要信息

    BOOL  ACTS2200_AO_GetVoltRangeInfo(	                // 获得采样范围的各种信息(幅度、分辨率、极性)
        HANDLE                      hDevice,			        // 设备对象句柄,它由DEV_Create()函数创建
        U32                         nChannel,			        // AO物理通道号[0, 3],nSampleRange等于AO_SAMPRANGE_FROM_DEVICE时此参数起作用
        U32                         nSampleRange,		        // 由用户指定的采样范围挡位号[0, 5],若=AO_SAMPRANGE_FROM_DEVICE(0xFFFFFFFF),表示依据设备当前的挡位号取得采样范围信息,此参数需传实际值,操作同ACTS2200_AO_CH_PARAM的nSampleRange
        PACTS2200_AO_VOLT_RANGE_INFO pRangeInfo);               // 采样范围信息

    BOOL  ACTS2200_AO_GetRateInfo(			            // 获得采样速率信息
        HANDLE                      hDevice,			        // 设备对象句柄,它由DEV_Create()函数创建
        PACTS2200_AO_SAMP_RATE_INFO pRateInfo);                 // 采样速率信息

    BOOL  ACTS2200_AO_ScaleBinToVolt(		                // 根据指定采样范围将单个通道的采样原码数据量化成电压数据,返回实际量化的点数
        PACTS2200_AO_VOLT_RANGE_INFO pRangeInfo,                // 当前转换数据需要的采样范围信息
        F64                         fVoltArray[],		        // 电压缓冲区,用于返回量化后的电压数据,单位V
        I16                         nBinArray[],		        // 二进制原码缓冲区,用于传入量化前的原码数据,取值区间为[0, 4095], (单指某个通道的连续数据)
        U32                         nScaleSamps,		        // 请求量化的采样点数
        U32*                        pSampsScaled);		        // 返回实际量化的采样点数

    BOOL  ACTS2200_AO_ScaleVoltToBin(		                // 根据指定采样范围将单个通道的电压数据量化成采样原码数据,返回实际量化的点数
        PACTS2200_AO_VOLT_RANGE_INFO pRangeInfo,                // 当前转换数据需要的采样范围信息
        I16                         nBinArray[],		        // 原码缓冲区,用于返回量化后的原码数据,取值区间为[0, 4095], (单指某个通道的连续数据)
        F64                         fVoltArray[],		        // 电压缓冲区,用于传入量化前的电压数据,单位V
        U32                         nScaleSamps,		        // 请求量化的采样点数
        U32*                        pSampsScaled);		        // 返回实际量化的采样点数

    BOOL  ACTS2200_AO_VerifyParam(			            // 校验AO工作参数(Verify Parameter),建议在初始化AO参数前调用此函数校验各参数合法性
        HANDLE                      hDevice,			        // 设备对象句柄,它由DEV_Create()函数创建
        PACTS2200_AO_PARAM          pAOParam);	                // 待校验的AO工作参数

    BOOL  ACTS2200_AO_LoadParam(                          // 从ACTS2200.ini中加载AO参数
        HANDLE                      hDevice,
        PACTS2200_AO_PARAM          pAOParam);

    BOOL  ACTS2200_AO_SaveParam(                          // 保存AO参数至ACTS2200.ini
        HANDLE                      hDevice,
        PACTS2200_AO_PARAM          pAOParam);

    BOOL  ACTS2200_AO_ResetParam(                         // 将当前AO参数复位至出厂值
        HANDLE                      hDevice,
        PACTS2200_AO_PARAM          pAOParam);


    // ################################ DIO数字量实现函数 ################################
    BOOL  ACTS2200_DI_ReadDigitalOneU32(                  // 单点读取端口数据
        HANDLE                      hDevice,                    // 设备对象句柄，它由DEV_Create()函数创建
        U32                         nPort,                      // 端口号(参考设备说明书)
        U32                         nLineEn,                    // 线使能，Bit0-31分别对应Line0-31，只返回使能位对应的线输入状态(具体线数量参考设备说明书)
        U32*                        pPortData);                 // 读取的线状态,Bit0-31分别对应Line0-31(具体线数量参考设备说明书)

    BOOL  ACTS2200_DI_ReadDigitalOneLines(                // 单点读取端口线数据
        HANDLE                      hDevice,                    // 设备对象句柄，它由DEV_Create()函数创建
        U32                         nPort,                      // 端口号(参考设备说明书)
        U8                          bLineEn[32],                // 线使能，bLineEn[0-31]分别对应Line0-31，只返回使能位对应的线输入状态(具体线数量参考设备说明书)
        U8                          bLineData[32]);             // 读取的线状态,bLineEn[0-31]分别对应Line0-31(具体线数量参考设备说明书)

    BOOL  ACTS2200_DO_WriteDigitalOneU32(                 // 单点写入端口数据
        HANDLE                      hDevice,                    // 设备对象句柄，它由DEV_Create()函数创建
        U32                         nPort,                      // 端口号(参考设备说明书)
        U32                         nLineEn,                    // 线使能，Bit0-31分别对应Line0-31，只写入使能位对应的线输出状态(具体线数量参考设备说明书)
        U32                         nPortData);                 // 要输出的线状态,Bit0-31分别对应Line0-31(具体线数量参考设备说明书)

    BOOL  ACTS2200_DO_WriteDigitalOneLines(               // 单点写入端口线数据
        HANDLE                      hDevice,                    // 设备对象句柄，它由DEV_Create()函数创建
        U32                         nPort,                      // 端口号(参考设备说明书)
        U8                          bLineEn[32],                // 线使能，bLineEn[0-31]分别对应Line0-31，只写入使能位对应的线输出状态(具体线数量参考设备说明书)
        U8                          bLineData[32]);             // 要输出的线状态,bLineEn[0-31]分别对应Line0-31(具体线数量参考设备说明书)

    BOOL  ACTS2200_DIO_ReadBackDigitalOneU32(             // 单点回读取端口状态和数据
        HANDLE                      hDevice,                    // 设备对象句柄，它由DEV_Create()函数创建
        U32                         nPort,                      // 端口号(参考设备说明书)
        U32*                        pLineDir,                   // 线方向，Bit0-31分别对应Line0-31，返回对应的线的状态， 0表示输入，1表示输出(具体线数量参考设备说明书)
        U32*                        pPortData);                 // 回读线的状态(包含输入和输出),Bit0-31分别对应Line0-31，0代表低电平，1代表高电平(具体线数量参考设备说明书)

    // ================================ DIO辅助函数 ================================
    BOOL  ACTS2200_DIO_SetPowerOnState(		        	// 设置上电状态
        HANDLE                      hDevice,		        	// 设备对象句柄,它由DEV_Create()函数创建
        U32                         nPort,			        	// 端口号, 取值范围参考说明书
        U8                          nState[32]);	        	// 状态值，详见上述定义, nState[0]表示line0...

    BOOL  ACTS2200_DIO_GetPowerOnState(		        	// 获取上电状态
        HANDLE                      hDevice,		        	// 设备对象句柄,它由DEV_Create()函数创建
        U32                         nPort,			        	// 端口号, 取值范围参考说明书
        U8                          nState[32]);	        	// 状态值，详见上述定义, nState[0]表示line0...


    // ################################ CTR计数器实现函数 ################################
    // ================================ CTR任务操作函数 ================================
    HANDLE  ACTS2200_CTR_CreateTask(                      // 创建计数器任务
        HANDLE                      hDevice);   	        	// 设备对象句柄,它由DEV_Create()函数创建

    BOOL  ACTS2200_CTR_InitTask(                          // 初始化计数器任务，在启动前执行
        HANDLE                      hCTRTask);		        	// 计数器任务句柄,它由CTR_CreateTask()函数创建

    BOOL  ACTS2200_CTR_StartTask(                         // 启动计数器任务
        HANDLE                      hCTRTask);		        	// 计数器任务句柄,它由CTR_CreateTask()函数创建

    BOOL  ACTS2200_CTR_StopTask(                          // 停止计数器任务
        HANDLE                      hCTRTask);		        	// 计数器任务句柄,它由CTR_CreateTask()函数创建

    BOOL  ACTS2200_CTR_ReleaseTask(                       // 释放计数器任务
        HANDLE                      hCTRTask);		        	// 计数器任务句柄,它由CTR_CreateTask()函数创建

    BOOL  ACTS2200_CTR_WaitUntilTaskDone(		            // 采样任务结束前等待,返回TRUE表示采样任务结束
        HANDLE                      hCTRTask,                   // 计数器任务句柄,它由CTR_CreateTask()函数创建
        F64                         fTimeout);			        // 用于等待的时间，单位：秒(S)

    // ================================ CTR通道创建函数 ================================
    BOOL  ACTS2200_CI_CreateFreqChan(			            // 创建频率测量通道, GATE:被测信号输入引脚
        HANDLE                      hCTRTask,		        	// 计数器任务句柄,它由CTR_CreateTask()函数创建
        U32                         nChannel,		        	// CTR物理通道号[0, 1]
        F64                         fMinVal,		        	// 最小值是换算后测量的预期最小值。单位：默认为Hz, 另支持时钟滴答计数
        F64                         fMaxVal,		        	// 最大值是换算后测量的预期最大值。单位：默认为Hz, 另支持时钟滴答计数
        U32                         nUnits,			        	// 换算后的单位是要被使用的单位。单位：频率赫兹或时钟滴答,它不仅决定着fMinVal和fMaxVal的单位类型，同时也决定着测量结果的单位类型。
        U32                         nStartingEdge,			    // 被测信号的边沿，=0:下降沿测量，=1:上升沿测量
        U32                         nMeasMethod,	        	// 测量方法,=0:低频测量, =1:高频测量, =2:宽范围测量
        F64                         fMeasTime,		        	// 测量时间,单位：秒(S),仅当测量方法为高频测量(nMeasMethod=1)时有效,取值范围[0.00000002, 42.94967295]
        U32                         nDivisor);		        	// 分频器，仅当测量方法为宽范围测量(nMeasMethod=2)时有效,取值范围[2, 4294967295]

    BOOL  ACTS2200_CI_CreatePeriodChan(			        // 创建周期测量通道，获得脉冲的周期时间。GATE:被测信号输入引脚
        HANDLE                      hCTRTask,		        	// 计数器任务句柄,它由CTR_CreateTask()函数创建
        U32                         nChannel,		        	// CTR物理通道号[0, 1]
        F64                         fMinVal,		        	// 最小值是换算后测量的预期最小值。单位：默认为秒, 另支持时钟滴答计数
        F64                         fMaxVal,		        	// 最大值是换算后测量的预期最大值。单位：默认为秒, 另支持时钟滴答计数
        U32                         nUnits,			        	// 换算后的单位是要被使用的单位。单位：时间秒或时钟滴答,它不仅决定着fMinVal和fMaxVal的单位类型，同时也决定着测量结果的单位类型
        U32                         nStartingEdge,			    // 被测信号的边沿，=0:下降沿测量，=1:上升沿测量,详见上面的参数值常量定义
        U32                         nMeasMethod,	        	// 测量方法,=0:低频测量, =1:高频测量, =2:宽范围测量
        F64                         fMeasTime,		        	// 测量时间,单位：秒(S),仅当测量方法为高频测量(nMeasMethod=1)时有效,取值范围[0.00000002, 42.94967295]
        U32                         nDivisor);		        	// 分频器，仅当测量方法为宽范围测量(nMeasMethod=2)时有效,取值范围[2, 4294967295]

    BOOL  ACTS2200_CI_CreateCountEdgesChan(		        // 创建边沿计数通道。 SRC:边沿计数的源输入引脚; GATE:控制是否暂停输入引脚; AUX:控制计数方向输入引脚
        HANDLE                      hCTRTask,		        	// 计数器任务句柄,它由CTR_CreateTask()函数创建
        U32                         nChannel,		        	// CTR物理通道号[0, 1]
        U32                         nActiveEdge,			    // 被测信号的边沿，=0:下降沿测量，=1:上升沿测量,详见上面的参数值常量定义
        U32                         nInitialCount,	        	// 初始计数值, 取值范围为[0, 2^32-1]
        U32                         nCountDirection);           // 计数方向, =0:减计数，=1:加计数, =2:外部控制(来自AUX),详见上面的参数值常量定义

    BOOL  ACTS2200_CI_CreateTwoEdgeSepChan(	        	// 创建两边沿间隔测量通道(Two Edge Separation)，获得两个边沿之间的时间间隔, AUX:第一个有效边沿输入引脚; GATE:第二个有效边沿输入引脚
        HANDLE                      hCTRTask,		        	// 计数器任务句柄,它由CTR_CreateTask()函数创建
        U32                         nChannel,		        	// CTR物理通道号[0, 1]
        F64                         fMinVal,		        	// 最小值是换算后测量的预期最小值。单位：默认为秒, 另支持时钟滴答计数
        F64                         fMaxVal,		        	// 最大值是换算后测量的预期最大值。单位：默认为秒, 另支持时钟滴答计数
        U32                         nUnits,			        	// 换算后的单位是要被使用的单位。单位：时间秒或时钟滴答,它不仅决定着fMinVal和fMaxVal的单位类型，同时也决定着测量结果的单位类型。
        U32                         nFirstEdge,		        	// 测量时的第一个信号(即启动信号,来自SRC)的边沿, =0:下降沿， =1:上升沿
        U32                         nSecondEdge);	        	// 测量时的第二个信号(即停止信号,来自GATE)的边沿, =0:下降沿， =1:上升沿

    BOOL  ACTS2200_CI_CreatePulseWidthChan(	        	// 创建脉冲宽度测量通道,获得脉冲的低电平或高电平的宽度(时间或时钟滴答计数), GATE:被测信号输入引脚
        HANDLE                      hCTRTask,		        	// 计数器任务句柄,它由CTR_CreateTask()函数创建
        U32                         nChannel,		        	// CTR物理通道号[0, 1]
        F64                         fMinVal,		        	// 最小值是换算后测量的预期最小值。单位：默认为秒, 另支持时钟滴答计数
        F64                         fMaxVal,		        	// 最大值是换算后测量的预期最大值。单位：默认为秒, 另支持时钟滴答计数
        U32                         nUnits,			        	// 换算后的单位是要被使用的单位。单位：时间秒或时钟滴答,它不仅决定着fMinVal和fMaxVal的单位类型，同时也决定着测量结果的单位类型。
        U32                         nStartingEdge);	        	// 开始边沿, =0:测量脉冲的低电平时间(或滴答数), =1:测量脉冲的高电平时间(或滴答数)

    BOOL  ACTS2200_CI_CreateSemiPeriodChan(	        	// 创建半周期测量通道，半周期测量是对两个连续边沿的时间间隔进行测量，对于每个脉冲即有两个半周期存在，从上升沿开始测量。GATE:被测信号输入引脚
        HANDLE                      hCTRTask,		        	// 计数器任务句柄,它由CTR_CreateTask()函数创建
        U32                         nChannel,		        	// CTR物理通道号[0, 1]
        F64                         fMinVal,		        	// 最小值是换算后测量的预期最小值。单位：默认为秒, 另支持时钟滴答计数
        F64                         fMaxVal,		        	// 最大值是换算后测量的预期最大值。单位：默认为秒, 另支持时钟滴答计数
        U32                         nUnits);		        	// 换算后的单位是要被使用的单位。单位：时间秒或时钟滴答,它不仅决定着fMinVal和fMaxVal的单位类型，同时也决定着测量结果的单位类型  .

    BOOL  ACTS2200_CI_CreatePulseChanFreq(	        	// 创建脉冲测量通道(基于频率),获得脉冲的成对的频率和占空比。GATE:被测信号输入引脚
        HANDLE                      hCTRTask,		        	// 计数器任务句柄,它由CTR_CreateTask()函数创建
        U32                         nChannel,		        	// CTR物理通道号[0, 1]
        F64                         fMinVal,		        	// 最小值是换算后测量的预期最小值。单位：默认为赫兹(Hz)
        F64                         fMaxVal,		        	// 最大值是换算后测量的预期最大值。单位：默认为Hz
        U32                         nUnits);		        	// 单位：赫兹(Hz)(仅支持赫兹)

    BOOL  ACTS2200_CI_CreatePulseChanTime(	        	// 创建脉冲测量通道(基于时间),获得脉冲的成对的高电平时间和低电平时间。GATE:被测信号输入引脚
        HANDLE                      hCTRTask,		        	// 计数器任务句柄,它由CTR_CreateTask()函数创建
        U32                         nChannel,		        	// CTR物理通道号[0, 1]
        F64                         fMinVal,		        	// 最小值是换算后测量的预期最小值。单位：默认为s, 它依赖于nUnits的指定
        F64                         fMaxVal,		        	// 最大值是换算后测量的预期最大值。单位：默认为s, 它依赖于nUnits的指定
        U32                         nUnits);		        	// 单位: 秒(S)(仅支持秒)

    BOOL  ACTS2200_CI_CreatePulseChanTicks(	        	// 创建脉冲测量通道(基于时钟滴答),获得脉冲的成对的高电平的时钟滴数量和低电平的时钟滴数量。GATE:被测信号输入引脚
        HANDLE                      hCTRTask,		        	// 计数器任务句柄,它由CTR_CreateTask()函数创建
        U32                         nChannel,		        	// CTR物理通道号[0, 1]
        U32                         nSrcTerminal,        	    // 基准时钟源
        F64                         fMinVal,		        	// 最小值是换算后测量的预期最小值。单位：个
        F64                         fMaxVal);		        	// 最大值是换算后测量的预期最大值。单位：个

    BOOL  ACTS2200_CI_CreateLinEncoderChan(	        	// 创建线性编码器测量通道。A:编码器通道A输入引脚; B:编码器通道B输入引脚; Z:编码器通道Z输入引脚
        HANDLE                      hCTRTask,		        	// 计数器任务句柄,它由CTR_CreateTask()函数创建
        U32                         nChannel,		        	// CTR物理通道号[0, 1]
        U32                         nDecodingType,	        	// 解码类型, 0=X1, 1=X2, 2=X4, 3=双脉冲
        BOOL                        bZidxEnable,	        	// 启用Z索引指定了是否为测量启用z索引。=TRUE:启动, =FALSE:禁用，如果启用的话，信号来自Z输入引脚
        F64                         fZidxVal,		        	// Z索引值, Z索引值指定当信号Z为高且信号A和信号B处于Z索引相位中指定的状态时测量的值，使用单位(nUnits)中指定的单位。
        U32                         nZidxPhase,		        	// Z索引相位, 详见上面的常量定义
        U32                         nUnits,			        	// 换算后的单位指定用于测量的单位。详见上面的常量定义
        F64                         fDistPerPulse,	        	// 脉冲间隔是编码器生成的每个脉冲之间的距离。该值使用由单位(nUnits)所指定的单位。
        F64                         fInitialPos);	        	// 初始位置是测量开始时编码器的位置。该值的单位由单位(nUnits)指定。

    BOOL  ACTS2200_CI_CreateAngEncoderChan(	        	// 创建角度编码器测量通道。A:编码器通道A输入引脚; B:编码器通道B输入引脚; Z:编码器通道Z输入引脚
        HANDLE                      hCTRTask,		        	// 计数器任务句柄,它由CTR_CreateTask()函数创建
        U32                         nChannel,		        	// CTR物理通道号[0, 1]
        U32                         nDecodingType,	        	// 解码类型, 0=X1, 1=X2, 2=X4, 3=双脉冲，4＝单脉冲
        BOOL                        bZidxEnable,	        	// 启用Z索引指定了是否为测量启用z索引。=TRUE:启动, =FALSE:禁用，如果启用的话，信号来自Z输入引脚
        F64                         fZidxVal,		        	// Z索引值, Z索引值指定当信号Z为高且信号A和信号B处于Z索引相位中指定的状态时测量的值，使用单位(nUnits)中指定的单位。
        U32                         nZidxPhase,		        	// Z索引相位, 详见上面的常量定义
        U32                         nUnits,			        	// 换算后的单位指定用于测量的单位。详见上面的常量定义
        U32                         nPulsesPerRev,	        	// 编码器每转一圈所生成脉冲的数量。该值是A信号或B信号上的脉冲数量，而不是A信号与B信号上总的脉冲数量。
        F64                         fInitialAngle);	        	// 初始角是编码器在开始时的角度。该值的单位由单位(nUnits)指定。

    BOOL  ACTS2200_CO_CreatePulseChanFreq(		        // 创建脉冲频率生成通道。OUT:脉冲输出引脚
        HANDLE                      hCTRTask,		        	// 计数器任务句柄,它由CTR_CreateTask()函数创建
        U32                         nChannel,		        	// CTR物理通道号[0, 1]
        U32                         nUnits,			        	// 指定参数fFreq的单位，默认为Hz
        U32                         nIdleState,		        	// 空闲时的状态，即在StartTask()前和StopTask()后的状态，=0:低电平, =1:高电平
        F64                         fInitialDelay,	        	// 初始延迟时间，即在任务开始(开始触发)往后到输出脉冲之间的时间值(单位：秒)
        F64                         fFreq,			        	// 生成脉冲的频率
        F64                         fDutyCycle);	        	// 生成脉冲的占空比，取值范围[0 < fDutyCycle < 1]

    BOOL  ACTS2200_CO_CreatePulseChanTime(		        // 创建脉冲时间生成通道。OUT:脉冲输出引脚
        HANDLE                      hCTRTask,		        	// 计数器任务句柄,它由CTR_CreateTask()函数创建
        U32                         nChannel,		        	// CTR物理通道号[0, 1]
        U32                         nUnits,			        	// 指定参数fLowTime和fHighTime的单位，默认为:秒
        U32                         nIdleState,		        	// 空闲时的状态，即在StartTask()前和StopTask()后的状态，=0:低电平, =1:高电平
        F64                         fInitialDelay,	        	// 初始延迟时间，即在任务开始(开始触发)往后到输出脉冲之间的时间值,单位：秒
        F64                         fLowTime,		        	// 脉冲低电平的时间,时间单位由nUnits指定
        F64                         fHighTime);		        	// 脉冲高电平的时间,时间单位由nUnits指定

    BOOL  ACTS2200_CO_CreatePulseChanTicks(	        	// 创建脉冲滴答生成通道。OUT:脉冲输出引脚
        HANDLE                      hCTRTask,		            // 计数器任务句柄,它由CTR_CreateTask()函数创建
        U32                         nChannel,		        	// CTR物理通道号[0, 1]
        U32                         nSrcTerminal,        	    // 基准时钟源
        U32                         nIdleState,		        	// 空闲时的状态，即在StartTask()前和StopTask()后的状态，=0:低电平, =1:高电平
        U32                         nInitialDelay,	        	// 初始延迟滴答，即在任务开始(开始触发)往后到输出脉冲之间的滴答值
        U32                         nLowTicks,		        	// 低电平的时钟滴答计数
        U32                         nHighTicks);	        	// 高电平的时钟滴答计数

    // ================================ CTR配置函数 ================================
    BOOL  ACTS2200_CTR_CfgImplicitTiming(		            // 配置隐式时钟(无须指定时钟源和采样速率,如半周期测量、频率测量,内部使用固定的基准时钟)
        HANDLE                      hCTRTask,		            // 计数器任务句柄,它由CTR_CreateTask()函数创建
        I32                         nSampleMode,		        // 采样模式
        U64                         nSampsPerChan);		        // 每通道采样点数

    BOOL  ACTS2200_CTR_DisableStartTrig(		            // 禁止开始触发
        HANDLE                      hCTRTask);		            // 计数器任务句柄,它由CTR_CreateTask()函数创建

    BOOL  ACTS2200_CTR_CfgDigEdgeStartTrig(		        // 配置开始触发(数字边沿)
        HANDLE                      hCTRTask,		            // 计数器任务句柄,它由CTR_CreateTask()函数创建
        U32                         nTrigSrc,		            // 数字触发源, 详见(DI DO CTR 采样时钟、开始触发源、暂停触发源、计数复位信号源使用的选项)
        U32                         nTrigEdge,		            // 数字触发边沿
        F64                         fDigFltrMinPulseWidth);	    // 数字触发允许最小脉冲宽度,单位：秒(S)，取值[0,42.94967295]);

    BOOL  ACTS2200_CTR_SetStartTrigReTrig(                // 允许开始触发的重触发
        HANDLE                      hCTRTask,		            // 计数器任务句柄,它由CTR_CreateTask()函数创建
        BOOL                        bEnable);	                // =FALSE:禁止重触发(默认), =TRUE:允许重触发

    BOOL  ACTS2200_CTR_SetInitialDelayOnReTrig(	        // 设置在触发中是否有延时状态
        HANDLE                      hCTRTask,		            // 计数器任务句柄,它由CTR_CreateTask()函数创建
        BOOL                        bEnable);                   // =FALSE:禁止在重触发事件后延时(默认), =TRUE:允许在重触发事件后延时

    BOOL  ACTS2200_CTR_DisablePauseTrig(		            // 禁止暂停触发
        HANDLE                      hCTRTask);		            // 计数器任务句柄,它由CTR_CreateTask()函数创建

    BOOL  ACTS2200_CTR_CfgDigLvlPauseTrig(		        // 配置暂停触发(数字边沿)
        HANDLE                      hCTRTask,		            // 计数器任务句柄,它由CTR_CreateTask()函数创建
        U32                         nTrigSrc,	            	// 数字触发源, 详见(DI DO CTR 采样时钟、开始触发源、暂停触发源、计数复位信号源使用的选项)
        U32                         nTrigWhen,	            	// 数字触发时机，0=低电平暂停触发，1=高电平暂停触发
        F64                         fDigFltrMinPulseWidth);	    // 数字触发允许最小脉冲宽度,单位：秒(S)，取值[0,42.94967295]);

    BOOL  ACTS2200_CTR_SendSoftTrig(			            // 发送软件触发事件(Send Software Trig),软件触发也叫强制触发
        HANDLE                      hCTRTask);		            // 计数器任务句柄,它由CTR_CreateTask()函数创建

    BOOL  ACTS2200_CI_SetDigitalFilter(			        // 设置CI数字滤波
        HANDLE                      hCTRTask,		            // 计数器任务句柄,它由CTR_CreateTask()函数创建
        U32                         nSignal,			        // 信号选择，0=Src，1=Gate, 2=Aux,详见前面的常量定义
        F64                         fDigFltrMinPulseWidth,		// 数字滤波允许最小脉冲宽度,可由nUnits参数指定其单位类型,若等于0.0则表示禁用数字滤波
        U32                         nUnits);			        // 指定滤波值fFilter的单位。详见上面的常量定义

    BOOL  ACTS2200_CI_SetCountEdgesCountResetParam(		// 设置CI边沿计数复位参数
        HANDLE                      hCTRTask,		            // 计数器任务句柄,它由CTR_CreateTask()函数创建
        BOOL                        bResetEnable,		        // 重置使能：0=Disable, 1=Enable
        U32                         nActiveEdge,		        // 有效边沿：0=Falling edge, 1=Rising edge
        U32                         nResetCount,		        // 复位计数值，32Bit有效
        U32                         nResetTerm,			        // 复位信号接入端子，, 详见(DI DO CTR 采样时钟、开始触发源、暂停触发源、计数复位信号源使用的选项)
        F64                         fDigFltrMinPulseWidth);	    // 数字滤波最小脉冲宽度,单位：秒(S)，取值[0,42.94967295]

    BOOL  ACTS2200_CI_CfgOutputEvent(                     // 设置CTR导出输出事件参数(即在CI边沿计数和编码器测量溢出时输出状态至OUT)
        HANDLE                      hCTRTask,		            // 计数器任务句柄,它由CTR_CreateTask()函数创建
        BOOL                        bEnable,		        	// 导出输出事件使能：0=Disable, 1=Enable
        U32                         nBehavior,		        	// 导出行为，0=Pulse(脉冲)，1=Toggle(反转)
        U32                         nPolarity,		        	// 脉冲极性，0=Low(低)，1=High(高),当nBehavior=Pulse时该参数有效
        U32                         nIdleState);	        	// 空闲状态，0=Low(低)，1=High(高),当nBehavior=Toggle时该参数有效

    // ================================ CTR读取函数 ================================
    BOOL  ACTS2200_CI_ReadCounterScalarU32(		        // 读取经过换算后的计数器结果数据(原码数据)，比如脉冲计数结果,主要应用于单点采样中(超时返回FALSE)
        HANDLE                      hCTRTask,		            // 计数器任务句柄,它由CTR_CreateTask()函数创建
        U32*                        pValue,			            // 返回经过换算后的单点测量数据
        U32*                        pReserved,			        // 保留
        F64                         fTimeout);			        // 超时时间，单位：秒(S)

    BOOL  ACTS2200_CI_ReadCounterScalarF64(		        // 读取经过换算后的计数器结果数据(物理量)，比如频率、周期测量结果,主要应用于单点采样中(超时返回FALSE)
        HANDLE                      hCTRTask,		            // 计数器任务句柄,它由CTR_CreateTask()函数创建
        F64*                        pValue,			            // 返回经过换算后单点测量结果数据
        F64*                        pReserved,			        // 保留
        F64                         fTimeout);			        // 超时时间，单位：秒(S)

    BOOL  ACTS2200_CI_ReadCtrFreqScalar(		            // 读取经过换算后的计数器结果数据(频率和占空比)(超时返回FALSE)
        HANDLE                      hCTRTask,		            // 计数器任务句柄,它由CTR_CreateTask()函数创建
        F64*                        pFrequency,		            // 返回单点的频率值,单位由函数CI_CreatePulseChanFreq()的参数nUnits决定
        F64*                        pDutyCycle,		            // 返回单点的占空比，取值范围(0.0, 1.0),是脉冲的高电平时间除以脉冲周期的结果
        F64                         fTimeout);			        // 超时时间，单位：秒(S)

    BOOL  ACTS2200_CI_ReadCtrTimeScalar(		            // 读取经过换算后的计数器结果数据((高电平和低电平时钟滴答))(超时返回FALSE)
        HANDLE                      hCTRTask,		            // 计数器任务句柄,它由CTR_CreateTask()函数创建
        F64*                        pHighTime,			        // 返回单点的高电平时间,单位由函数CI_CreatePulseChanTime()的参数nUnits决定
        F64*                        pLowTime,			        // 返回单点的低电平时间,单位由函数CI_CreatePulseChanTime()的参数nUnits决定
        F64                         fTimeout);			        // 超时时间，单位：秒(S)

    BOOL  ACTS2200_CI_ReadCtrTicksScalar(		            // 读取经过换算后的计数器结果数据(高电平和低电平时钟滴答)(超时返回FALSE)
        HANDLE                      hCTRTask,		            // 计数器任务句柄,它由CTR_CreateTask()函数创建
        U32*                        pHighTicks,		            // 返回单点的高电平滴答
        U32*                        pLowTicks,			        // 返回单点的低电平滴答
        F64                         fTimeout);			        // 超时时间，单位：秒(S), 大于0.0表示指定超时时间且等待，若完成，等于0.0表示不使用超时等待立即执行并返回，小于0.0表示不指定超时间且等待直到请求被执行完成后返回

    // ================================ CTR写入函数 ================================
    BOOL  ACTS2200_CO_WriteFreqScalar(				    // 向生成任务中写入单次频率和占空比数据
        HANDLE                      hCTRTask,		            // 计数器任务句柄,它由CTR_CreateTask()函数创建
        F64                         fFreq,		                // 频率数据,单位由函数CO_CreatePulseChanFreq()的参数nUnits指定
        F64                         fDutyCycle,	                // 占空比数据，取值范围[0 < fDutyCycle < 1]
        F64                         fTimeout);			        // 超时时间，单位：秒(S)

    BOOL  ACTS2200_CO_WriteTimeScalar(				    // 向生成任务中写入单次时间数据
        HANDLE                      hCTRTask,		            // 计数器任务句柄,它由CTR_CreateTask()函数创建
        F64                         fHighTime,	                // 高电平时间数据,单位由函数CO_CreatePulseChanTime()的参数nUnits指定
        F64                         fLowTime,	                // 低电平时间数据,单位由函数CO_CreatePulseChanTime()的参数nUnits指定
        F64                         fTimeout);			        // 超时时间，单位：秒(S)

    BOOL  ACTS2200_CO_WriteTicksScalar(				    // 向生成任务中写入单次时钟滴答数据
        HANDLE                      hCTRTask,		            // 计数器任务句柄,它由CTR_CreateTask()函数创建
        U32                         nHighTicks,	                // 高电平的时钟滴答数据
        U32                         nLowTicks,	                // 低电平的时钟滴答数据
        F64                         fTimeout);			        // 超时时间，单位：秒(S)

    // ================================ CTR辅助函数 ================================
    BOOL  ACTS2200_CTR_GetTimeBaseRate(				    // 获得任务使用的基准频率
        HANDLE                      hCTRTask,		            // 计数器任务句柄,它由CTR_CreateTask()函数创建
        F64*                        fTimeBaseRate);	            // 返回基准频率

    BOOL  ACTS2200_CO_GetCountVal(				        // 取得CO的当前输出计数值
        HANDLE                      hCTRTask,		            // 计数器任务句柄,它由CTR_CreateTask()函数创建
        U32*                        pCountVal);		            // 返回当前CO的输出计数值

    BOOL  ACTS2200_CO_GetOutputState(			            // 取得CO当前的输出状态
        HANDLE                      hCTRTask,		            // 计数器任务句柄,它由CTR_CreateTask()函数创建
        U32*                        pOutputState);		        // 返回CO当前输出状态, 0=Low, 1=High


//#ifdef __cplusplus
//}
//#endif

#endif // _ART_ACTS2200_H_
