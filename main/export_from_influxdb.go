package main

import (
	"context"
	"errors"
	"flag"
	"fmt"
	"github.com/gocarina/gocsv"
	influxdb2 "github.com/influxdata/influxdb-client-go/v2"
	"os"
	"time"
)

type record struct {
	T time.Time `csv:"_time"`
	V float64   `csv:"_value"`
}

const (
	Default_Begin_Time = "2022-03-10T15:00:00Z"
	Time_Format        = "2006-01-02T15:04:05Z"
)

func Exists(name string) bool {
	_, err := os.Stat(name)
	if err == nil {
		return true
	}
	if errors.Is(err, os.ErrNotExist) {
		return false
	}
	panic(err)
}

func main() {
	beginTimeStr := flag.String("b", Default_Begin_Time, "Begin Time , format 2006-01-02T15:04:05Z")
	hourNumber := flag.Int("h", 1, "hour for number to get, default is 1 hour")

	client := influxdb2.NewClient("http://127.0.0.1:8086", "2icFg9HTzq6viq-rzoBfW_In6ZKM6BGu2RtPfgMaZAu0QVtMQndm4PImpwBEDjR0R_V4ru8H8wc9ocbtJT9x5A==")
	// always close client at the end
	defer client.Close()

	// Get query client
	queryAPI := client.QueryAPI("znb")

	beginTime, err := time.Parse(Time_Format, *beginTimeStr)
	if err != nil {
		panic(err)
	}
	beginTime = beginTime.Add(-8 * time.Hour)

	for h := 0; h < *hourNumber; h++ {
		for channel := 1; channel <= 6; channel++ {
			b := beginTime.Add(time.Duration(h) * time.Hour)

			csvFileName := fmt.Sprintf("/home/ubuntu/data/csv/channel-%d-%s.csv", channel, b.Add(8*time.Hour).Format(Time_Format))
			gzFileName := csvFileName + ".gz"

			if Exists(csvFileName) {
				println(fmt.Sprintf("csv file: %s exists", csvFileName))
				continue
			}

			if Exists(gzFileName) {
				println(fmt.Sprintf("gz file: %s exists", gzFileName))
				continue
			}

			start := time.Now()

			e := b.Add(1 * time.Minute)
			var records []record

			i := 0
			for j := 0; j < 60; j++ {
				query := fmt.Sprintf(`from(bucket: "znb") |> range(start: %s, stop: %s) |> filter(fn: (r) => r["_measurement"] == "Ampere")   |> filter(fn: (r) => r["channel"] == "%d") |> keep(columns: ["_time", "_value"])`,
					b.Format(Time_Format), e.Format(Time_Format), channel)
				//query := `from(bucket: "znb") |> range(start: 2022-02-11T04:00:00Z, stop: 2022-02-11T04:01:00Z) |> filter(fn: (r) => r["_measurement"] == "Ampere")  |> keep(columns: ["_time", "_value"])`
				//query := `from(bucket: "znb") |> range(start: 2022-02-15T00:40:00Z, stop: 2022-02-15T00:41:00Z) |> filter(fn: (r) => r["_measurement"] == "Ampere") |> filter(fn: (r) => r["channel"] == "1")`

				// get QueryTableResult
				result, err1 := queryAPI.Query(context.Background(), query)
				if err1 != nil {
					panic(err1)
				}

				// Iterate over query response
				for result.Next() {
					// Notice when group key has changed
					if result.TableChanged() {
						fmt.Printf("table: %s\n", result.TableMetadata().String())
					}
					i++
					//// Access data
					records = append(records, record{
						T: result.Record().Time().Add(8 * time.Hour),
						V: result.Record().Value().(float64),
					})
				}

				// check for an error
				if result.Err() != nil {
					fmt.Printf("query parsing error: %v\n", result.Err().Error())
				}

				b = b.Add(1 * time.Minute)
				e = e.Add(1 * time.Minute)
			}

			csvFile, err := os.OpenFile(csvFileName, os.O_RDWR|os.O_CREATE, os.ModePerm)
			if err != nil {
				panic(err)
			}

			err = gocsv.MarshalFile(&records, csvFile) // Use this to save the CSV back to the file
			if err != nil {
				panic(err)
			}
			csvFile.Close()

			end := time.Now()
			elapsed := end.Sub(start)
			fmt.Printf("write csv %s, %d line elapsed: %v\n", csvFileName, len(records), elapsed)
		}
	}
}
