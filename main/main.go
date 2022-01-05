package main

import "github.com/peace0phmind/art_go"

func main() {
	art, err := art_go.NewArt(0)
	if err != nil {
		print(err)
	} else {
		param := &art_go.AITaskParam{}
		param.SampleSignal = art_go.AI_SAMPLE_SIGNAL_AI
		// 时间参数
		param.SampleMode = art_go.SAMPLE_MODE_CONTINUOUS
		param.SamplePerChan = 1000
		param.SampleRate = 1000
		param.SampleClockSource = art_go.SAMPLE_CLOCK_SOURCE_LOCAL
		param.ExtSampleClockEdge = art_go.Ext_SAMPLE_CLOCK_EDGE_RISING

		for i := 0; i < 4; i++ {
			ch := art_go.AIChannelParam{}
			ch.Channel = i
			ch.SampleRange = art_go.SAMPLE_RANGE_N10_P10V
			ch.RefGround = art_go.REF_GROUND_RSE
		}

		task, err1 := art.InitAITask(param)
		if err1 != nil {
			print(err1)
		} else {
			println(task)
		}

		art.Release()
		println("create ok")
	}
}
