package main

import (
	"fmt"
	"github.com/peace0phmind/art_go"
	"time"
)

func main() {
	art, err := art_go.NewArt(0)
	if err != nil {
		println(fmt.Sprintf("create art err: %v", err))
	} else {
		defer art.Release()

		param := &art_go.AITaskParam{}
		param.SampleSignal = art_go.AI_SAMPLE_SIGNAL_AI
		// 时间参数
		param.SampleMode = art_go.SAMPLE_MODE_CONTINUOUS
		param.SamplePerChannel = 5000
		param.SampleRate = 10000
		param.SampleClockSource = art_go.SAMPLE_CLOCK_SOURCE_LOCAL
		param.ExtSampleClockEdge = art_go.Ext_SAMPLE_CLOCK_EDGE_RISING

		for i := 0; i < 4; i++ {
			ch := art_go.AIChannelParam{}
			ch.Channel = i
			ch.SampleRange = art_go.SAMPLE_RANGE_N10_P10V
			ch.RefGround = art_go.REF_GROUND_RSE
			param.Channels = append(param.Channels, ch)
		}

		task, err := art.InitAITask(param)
		if err != nil {
			println(fmt.Sprintf("init ai task err: %v", err))
		} else {
			if err = task.Start(); err != nil {
				println(fmt.Sprintf("start ai task err: %v", err))
			} else {
				if err = task.SendSoftTrig(); err != nil {
					println(fmt.Sprintf("send soft trig err: %v", err))
				} else {
					buf := make([]float64, 4*param.SamplePerChannel)
					for i := 0; i < 1000; i++ {
						count, err := task.ReadAnalog(&buf, 10, art_go.FILL_MODE_GroupByScanNumber)
						println(fmt.Sprintf("%s get count: %v", time.Now().Format("0405.999999999"), count))
						if err != nil {
							println(fmt.Sprintf("read analog error: %v", err))
						} else {
							println(fmt.Sprintf("%v, %v, %v, %v", buf[0], buf[1], buf[2], buf[3]))
						}
					}
				}

				task.Stop()
				task.Release()
			}
		}

		println("create ok")
	}
}
