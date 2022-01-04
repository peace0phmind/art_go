package art_go

/*
#include "./libs/ACTS2200.h"

#cgo arm64 LDFLAGS: -L${SRCDIR}/libs/arm -lACTS2200
#cgo amd64 LDFLAGS: -L${SRCDIR}/libs/x86 -lACTS2200
*/
import "C"
import "errors"

type Art struct {
	handle C.HANDLE
}

func NewArt(nSerialCode uint32) (*Art, error) {
	ret := &Art{}
	ret.handle = C.ACTS2200_DEV_Create(C.uint(nSerialCode))
	if ret.handle == nil {
		return nil, errors.New("dev create error")
	}

	return ret, nil
}

func (art *Art) AIInitTask() error {
	//
	//C.struct_AIParam.nSampChanCount = 4
	//AIParam.nSampSignal = C.ACTS2200_AI_SAMPSIGNAL_0V
	//AIParam.nReserved0 = 0
	//AIParam.nReserved1 = 0
	//for (nChannel = 0; nChannel < ACTS2200_AI_MAX_CHANNELS; nChannel++)
	//{
	//AIParam.CHParam[nChannel].nChannel = nChannel;
	//AIParam.CHParam[nChannel].nSampRange = 0;
	//AIParam.CHParam[nChannel].nRefGround    = ACTS2200_AI_REFGND_RSE;
	//
	//AIParam.CHParam[nChannel].nReserved0 = 0;
	//AIParam.CHParam[nChannel].nReserved1 = 0;
	//AIParam.CHParam[nChannel].nReserved2 = 0;
	//}
	//
	//// 时钟参数
	//AIParam.nSampMode = ACTS2200_SAMPMODE_CONTINUOUS
	//AIParam.nSampsPerChan = nReadSampsPerChan
	//AIParam.fSampRate = 25000
	//AIParam.nExtSampClkEdge = 1
	//AIParam.nReserved3 = 0
	//
	//C.ACTS2200_AI_InitTask()
	return nil
}
