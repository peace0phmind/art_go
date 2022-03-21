package main

import (
	"fmt"
	"github.com/gocarina/gocsv"
	"github.com/influxdata/influxdb-client-go/v2"
	"github.com/influxdata/influxdb-client-go/v2/api"
	"github.com/influxdata/influxdb-client-go/v2/api/http"
	"github.com/peace0phmind/art_go"
	"os"
	"strconv"
	"sync"
	"time"
)

type AmpereValue struct {
	Channel int       `csv:"channel"`
	Time    time.Time `csv:"time"`
	Value   float64   `csv:"value"`
}

var ampereValuePool sync.Pool

func getAmpereValue() *AmpereValue {
	v := ampereValuePool.Get()
	if v == nil {
		v = &AmpereValue{}
	}

	return v.(*AmpereValue)
}

func putAmpereValue(v *AmpereValue) {
	ampereValuePool.Put(v)
}

func putAllAmpereValue(vs []*AmpereValue) {
	for v := range vs {
		ampereValuePool.Put(v)
	}
}

const channel_count = 16
const channel_count_for_save = 6

func printInfluxdbError(writeAPI api.WriteAPI) {
	for e := range writeAPI.Errors() {
		println(e)
	}
}

func saveAmpereToInfluxdb(vvc chan *AmpereValue, writeAPI api.WriteAPI) {
	for v := range vvc {
		p := influxdb2.NewPointWithMeasurement("Ampere").
			AddTag("channel", strconv.Itoa(v.Channel+1)).
			AddField("value", v.Value).
			SetTime(v.Time)
		writeAPI.WritePoint(p)
		putAmpereValue(v)
	}

	writeAPI.Flush()
}

const (
	CSV_File_Format      = "15_04_05"
	CSV_File_Path_Format = "2006_01_02"
)

func saveAmpereToCsv(vvc chan *AmpereValue) {
	var ampereValues []*AmpereValue
	for v := range vvc {
		if v.Channel == 0 && v.Time.Second() == 0 && v.Time.Nanosecond() == 0 && len(ampereValues) > 0 {
			// save to csv file
			tt := v.Time.Add(-1 * time.Minute)
			filePath := fmt.Sprintf("/home/ubuntu/csv/%s", tt.Format(CSV_File_Path_Format))
			err := os.MkdirAll(filePath, os.ModePerm)
			if err != nil {
				println(err)
			}

			filename := fmt.Sprintf("%s/%s.csv", filePath, tt.Format(CSV_File_Format))
			csvFile, err := os.OpenFile(filename, os.O_RDWR|os.O_CREATE, os.ModePerm)
			if err != nil {
				println(err)
			}

			err = gocsv.MarshalFile(&ampereValues, csvFile) // Use this to save the CSV back to the file
			if err != nil {
				println(err)
			}

			//buf, err := gocsv.MarshalBytes(&ampereValues)
			//if err == nil {
			//	w := gzip.NewWriter(csvFile)
			//	_, e := w.Write(buf)
			//	if e != nil {
			//		println(e)
			//	}
			//	w.Close()
			//} else {
			//	println(err)
			//}

			csvFile.Close()

			putAllAmpereValue(ampereValues)
			ampereValues = nil
		}

		ampereValues = append(ampereValues, v)
	}
}

func main() {
	art, err := art_go.NewArt(0)
	if err != nil {
		println(fmt.Sprintf("create art err: %v", err))
	} else {
		defer art.Release()

		// open influx db
		client := influxdb2.NewClientWithOptions("http://127.0.0.1:8086",
			"2icFg9HTzq6viq-rzoBfW_In6ZKM6BGu2RtPfgMaZAu0QVtMQndm4PImpwBEDjR0R_V4ru8H8wc9ocbtJT9x5A==",
			influxdb2.DefaultOptions().SetUseGZip(true).SetPrecision(time.Millisecond).SetBatchSize(1000))
		defer client.Close()

		writeAPI := client.WriteAPI("znb", "znb")
		go printInfluxdbError(writeAPI)
		writeAPI.SetWriteFailedCallback(func(batch string, error http.Error, retryAttempts uint) bool {
			println(fmt.Sprintf("do retry: %s, err: %v", batch, error))
			return true
		})

		// init chan task
		ampereValueChan := make(chan *AmpereValue, 1000*32*600)
		//go saveAmpereToInfluxdb(ampereValueChan, writeAPI)
		go saveAmpereToCsv(ampereValueChan)

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
							//if dd.Hour() < 6 || dd.Hour() > 22 {
							//	continue
							//}

							for j := 0; j < param.SamplePerChannel; j++ {
								for c := 0; c < channel_count_for_save; c++ {
									v := getAmpereValue()
									v.Channel = c
									v.Value = buf[channel_count*j+c]
									v.Time = time.Unix(dd.Unix(), int64(1000000*j))
									ampereValueChan <- v
								}
							}
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
