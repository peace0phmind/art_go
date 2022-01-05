package art_go

/*
#include "./libs/ACTS2200.h"

#cgo arm64 LDFLAGS: -L${SRCDIR}/libs/arm -Wl,-rpath=${SRCDIR}/libs/arm -lACTS2200
#cgo amd64 LDFLAGS: -L${SRCDIR}/libs/x86 -Wl,-rpath=${SRCDIR}/libs/x86 -lACTS2200
*/
import "C"
import (
	"errors"
	"fmt"
)

type Art struct {
	handle C.HANDLE
}

type AITask struct {
	art *Art
}

const (
	// 采样范围
	SAMPLE_RANGE_N10_P10V   = C.ACTS2200_AI_SAMPRANGE_N10_P10V   // ±10V
	SAMPLE_RANGE_N5_P5V     = C.ACTS2200_AI_SAMPRANGE_N5_P5V     // ±5V
	SAMPLE_RANGE_N2_P2V     = C.ACTS2200_AI_SAMPRANGE_N2_P2V     // ±2V
	SAMPLE_RANGE_N1_P1V     = C.ACTS2200_AI_SAMPRANGE_N1_P1V     // ±1V
	SAMPLE_RANGE_N2P5_P2P5V = C.ACTS2200_AI_SAMPRANGE_N2P5_P2P5V // ±2.5V 保留(未用)
	SAMPLE_RANGE_0V_P10V    = C.ACTS2200_AI_SAMPRANGE_0V_P10V    // 0-10V  保留(未用)
	SAMPLE_RANGE_MAX_RANGES = C.ACTS2200_AI_MAX_RANGES

	// ref ground 参数所使用的接地参考选项
	REF_GROUND_RSE  = C.ACTS2200_AI_REFGND_RSE  // 接地参考单端(Referenced Single Endpoint)
	REF_GROUND_NRSE = C.ACTS2200_AI_REFGND_NRSE // 非参考单端(Non Referenced Single Endpoint)
	REF_GROUND_DIFF = C.ACTS2200_AI_REFGND_DIFF // 差分(Differential)
)

type AIChannelParam struct {
	Channel     int
	SampleRange int // 采样范围 (Sample Range)
	RefGround   int // 接地参考 (ref ground)
}

type AITaskParam struct {
	SampleSignal int // 采样信号(Sample Signal)
	// 时钟参数
	SampleMode         int              // 采样模式
	SamplePerChan      int              // 每个通道采样点数(也是每通道待读取点数),取值范围为单点采样模式时应等于1，有限点或连续采样时为[2, 1024*1024]
	SampleRate         float64          // 采样速率(Sample Rate), 单位为sps，取值范围为[0, 500K sps],它是每个采样通道的采样速率。
	SampleClockSource  int              // 采样时钟源(请详见下面的常量定义)
	ExtSampleClockEdge int              // 外部采样时钟边沿, 0=下降沿, 1=上升沿(仅当nSampClkSrc外部采样时钟源时有效)
	Channels           []AIChannelParam // 通道参数配置
}

const (
	// 采样信号 Sample Signal
	AI_SAMPLE_SIGNAL_AI = C.ACTS2200_AI_SAMPSIGNAL_AI

	// 采样模式, 0=单点采样(按需)， 1、硬件定时单点(暂不支持)，2=有限点采样， 3=连续采样
	SAMPLE_MODE_ONE_DEMAND = C.ACTS2200_SAMPMODE_ONE_DEMAND // 软件按需单点采样
	SAMPLE_MODE_FINITE     = C.ACTS2200_SAMPMODE_FINITE     // 有限点采样
	SAMPLE_MODE_CONTINUOUS = C.ACTS2200_SAMPMODE_CONTINUOUS // 连续采样

	// 采样时钟源
	SAMPLE_CLOCK_SOURCE_LOCAL = C.ACTS2200_VAL_LOCAL // 使用本地源

	// 外部采样时钟边沿
	Ext_SAMPLE_CLOCK_EDGE_FALLING = C.ACTS2200_VAL_FALLING // 下降沿
	Ext_SAMPLE_CLOCK_EDGE_RISING  = C.ACTS2200_VAL_RISING  // 上升沿
)

func NewArt(nSerialCode uint32) (*Art, error) {
	ret := &Art{}
	ret.handle = C.ACTS2200_DEV_Create(C.uint(nSerialCode))
	if ret.handle == nil {
		return nil, errors.New("dev create error")
	}

	return ret, nil
}

func (art *Art) Release() {
	if art.handle != nil {
		C.ACTS2200_DEV_Release(art.handle)
		art.handle = nil
	}
}

func (art *Art) InitAITask(param *AITaskParam) (*AITask, error) {
	if len(param.Channels) > C.ACTS2200_AI_MAX_CHANNELS {
		return nil, errors.New(fmt.Sprintf("param error, max channel is %d", C.ACTS2200_AI_MAX_CHANNELS))
	}

	aiTask := &AITask{
		art: art,
	}

	p := &C.struct__ACTS2200_AI_PARAM{}

	p.nSampChanCount = C.uint(len(param.Channels))
	p.nSampSignal = C.uint(param.SampleSignal)
	p.nReserved0 = 0
	p.nReserved1 = 0

	// 时钟参数
	p.nSampMode = C.uint(param.SampleMode)
	p.nSampsPerChan = C.uint(param.SamplePerChan)
	p.fSampRate = C.double(param.SampleRate)
	p.nSampClkSrc = C.uint(param.SampleClockSource)
	p.nExtSampClkEdge = C.uint(param.ExtSampleClockEdge)

	p.nReserved2 = 0
	p.nReserved3 = 0

	for i, ch := range param.Channels {
		p.CHParam[i].nChannel = C.uint(ch.Channel)
		p.CHParam[i].nSampRange = C.uint(ch.SampleRange)
		p.CHParam[i].nRefGround = C.uint(ch.RefGround)

		p.CHParam[i].nReserved0 = 0
		p.CHParam[i].nReserved1 = 0
		p.CHParam[i].nReserved2 = 0
	}

	initRet := C.ACTS2200_AI_InitTask(art.handle, p)
	if initRet != 0 {
		return aiTask, nil
	} else {
		return nil, errors.New("call init task fail")
	}
}
