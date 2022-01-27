package main

import (
	"fmt"
	"github.com/gocarina/gocsv"
	"github.com/peace0phmind/art_go"
	"os"
	"time"
)

type Csv struct { // Our example struct, you can use "-" to ignore a field
	Id  string  `csv:"date"`
	C1  float64 `csv:"c1"`
	C2  float64 `csv:"c2"`
	C3  float64 `csv:"c3"`
	C4  float64 `csv:"c4"`
	C5  float64 `csv:"c5"`
	C6  float64 `csv:"c6"`
	C7  float64 `csv:"c7"`
	C8  float64 `csv:"c8"`
	C9  float64 `csv:"c9"`
	C10 float64 `csv:"c10"`
	C11 float64 `csv:"c11"`
	C12 float64 `csv:"c12"`
	C13 float64 `csv:"c13"`
	C14 float64 `csv:"c14"`
	C15 float64 `csv:"c15"`
	C16 float64 `csv:"c16"`
}

const cannel_count = 16

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
		param.SamplePerChannel = 1000
		param.SampleRate = 1000
		param.SampleClockSource = art_go.SAMPLE_CLOCK_SOURCE_LOCAL
		param.ExtSampleClockEdge = art_go.Ext_SAMPLE_CLOCK_EDGE_RISING

		for i := 0; i < cannel_count; i++ {
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
					var csvs []Csv
					buf := make([]float64, cannel_count*param.SamplePerChannel)
					for i := 0; i < 20; i++ {
						count, err := task.ReadAnalog(&buf, 10, art_go.FILL_MODE_GroupByScanNumber)
						println(fmt.Sprintf("%s get count: %v", time.Now().Format("0405.999999999"), count))
						if err != nil {
							println(fmt.Sprintf("read analog error: %v", err))
						} else {
							dd := time.Now()

							for j := 0; j < param.SamplePerChannel; j++ {
								csvs = append(csvs, Csv{
									Id:  fmt.Sprintf("%s%03d", dd.Format("20060201150405"), j),
									C1:  buf[cannel_count*j+0],
									C2:  buf[cannel_count*j+1],
									C3:  buf[cannel_count*j+2],
									C4:  buf[cannel_count*j+3],
									C5:  buf[cannel_count*j+4],
									C6:  buf[cannel_count*j+5],
									C7:  buf[cannel_count*j+6],
									C8:  buf[cannel_count*j+7],
									C9:  buf[cannel_count*j+8],
									C10: buf[cannel_count*j+9],
									C11: buf[cannel_count*j+10],
									C12: buf[cannel_count*j+11],
									C13: buf[cannel_count*j+12],
									C14: buf[cannel_count*j+13],
									C15: buf[cannel_count*j+14],
									C16: buf[cannel_count*j+15],
								})
							}
						}
					}

					csvFile, err := os.OpenFile("clients.csv", os.O_RDWR|os.O_CREATE, os.ModePerm)
					if err != nil {
						panic(err)
					}
					defer csvFile.Close()

					err = gocsv.MarshalFile(&csvs, csvFile) // Use this to save the CSV back to the file
					if err != nil {
						panic(err)
					}

				}

				task.Stop()
				task.Release()
			}
		}

		println("create ok")
	}
}
