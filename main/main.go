package main

import (
	"fmt"
	"github.com/influxdata/influxdb-client-go/v2"
	"github.com/influxdata/influxdb-client-go/v2/api"
	"github.com/influxdata/influxdb-client-go/v2/api/http"
	"github.com/peace0phmind/art_go"
	"strconv"
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

const channel_count = 16
const channel_count_for_save = 15

func printInfluxdbError(writeAPI api.WriteAPI) {
	for e := range writeAPI.Errors() {
		println(e)
	}
}

func main() {
	art, err := art_go.NewArt(0)
	if err != nil {
		println(fmt.Sprintf("create art err: %v", err))
	} else {
		defer art.Release()

		// open influx db
		client := influxdb2.NewClientWithOptions("http://192.168.1.93:8086",
			"yvTRbUoJhVR4mI6GLCKwOX1pxczUZCWXiBEjCZ8dEq9W2KQatEPqQhLJxg5sj93c7XVmFTZf89V8f5yH3VVAgw==",
			influxdb2.DefaultOptions().SetUseGZip(true).SetPrecision(time.Millisecond).SetBatchSize(1000))
		defer client.Close()

		writeAPI := client.WriteAPI("znb", "znb")
		go printInfluxdbError(writeAPI)
		writeAPI.SetWriteFailedCallback(func(batch string, error http.Error, retryAttempts uint) bool {
			println(fmt.Sprintf("do retry: %s, err: %v", batch, error))
			return true
		})

		param := &art_go.AITaskParam{}
		param.SampleSignal = art_go.AI_SAMPLE_SIGNAL_AI
		// 时间参数
		param.SampleMode = art_go.SAMPLE_MODE_CONTINUOUS
		param.SamplePerChannel = 1000
		param.SampleRate = 1000
		param.SampleClockSource = art_go.SAMPLE_CLOCK_SOURCE_LOCAL
		param.ExtSampleClockEdge = art_go.Ext_SAMPLE_CLOCK_EDGE_RISING

		for i := 0; i < channel_count; i++ {
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
					//var csvs []Csv
					buf := make([]float64, channel_count*param.SamplePerChannel)
					//for i := 0; i < 20; i++ {
					for true {
						count, err := task.ReadAnalog(&buf, 10, art_go.FILL_MODE_GroupByScanNumber)
						println(fmt.Sprintf("%s get count: %v", time.Now().Format("2006-01-02T15:04:05.999999999Z"), count))
						if err != nil {
							println(fmt.Sprintf("read analog error: %v", err))
						} else {
							dd := time.Now()
							if dd.Hour() < 6 || dd.Hour() > 22 {
								continue
							}

							for j := 0; j < param.SamplePerChannel; j++ {
								for c := 0; c < channel_count_for_save; c++ {
									p := influxdb2.NewPointWithMeasurement("Ampere").
										AddTag("channel", strconv.Itoa(c+1)).
										AddField("value", buf[channel_count*j+c]).
										SetTime(time.Unix(dd.Unix(), int64(1000000*j)))
									// write asynchronously
									writeAPI.WritePoint(p)
								}

								//csvs = append(csvs, Csv{
								//	Id:  fmt.Sprintf("%s%03d", dd.Format("20060201150405"), j),
								//	C1:  buf[cannel_count*j+0],
								//	C2:  buf[cannel_count*j+1],
								//	C3:  buf[cannel_count*j+2],
								//	C4:  buf[cannel_count*j+3],
								//	C5:  buf[cannel_count*j+4],
								//	C6:  buf[cannel_count*j+5],
								//	C7:  buf[cannel_count*j+6],
								//	C8:  buf[cannel_count*j+7],
								//	C9:  buf[cannel_count*j+8],
								//	C10: buf[cannel_count*j+9],
								//	C11: buf[cannel_count*j+10],
								//	C12: buf[cannel_count*j+11],
								//	C13: buf[cannel_count*j+12],
								//	C14: buf[cannel_count*j+13],
								//	C15: buf[cannel_count*j+14],
								//	C16: buf[cannel_count*j+15],
								//})
							}

							writeAPI.Flush()
						}
					}

					//csvFile, err := os.OpenFile("clients.csv", os.O_RDWR|os.O_CREATE, os.ModePerm)
					//if err != nil {
					//	panic(err)
					//}
					//defer csvFile.Close()
					//
					//err = gocsv.MarshalFile(&csvs, csvFile) // Use this to save the CSV back to the file
					//if err != nil {
					//	panic(err)
					//}

				}

				task.Stop()
				task.Release()
			}
		}

		println("create ok")
	}
}
