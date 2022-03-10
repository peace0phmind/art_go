package main

import (
	"context"
	"fmt"
	"github.com/gocarina/gocsv"
	influxdb2 "github.com/influxdata/influxdb-client-go/v2"
	"os"
	"testing"
	"time"
)

type record struct {
	T time.Time `csv:"_time"`
	V float64   `csv:"_value"`
}

const (
	Begin_Time  = "2022-03-10T07:00:00Z"
	Time_Format = "2006-01-02T15:04:05Z"
)

func TestExportCsv(t *testing.T) {
	client := influxdb2.NewClient("http://127.0.0.1:8086", "2icFg9HTzq6viq-rzoBfW_In6ZKM6BGu2RtPfgMaZAu0QVtMQndm4PImpwBEDjR0R_V4ru8H8wc9ocbtJT9x5A==")
	// always close client at the end
	defer client.Close()

	// Get query client
	queryAPI := client.QueryAPI("znb")

	b, err := time.Parse(Time_Format, Begin_Time)
	if err != nil {
		panic(err)
	}
	e := b.Add(1 * time.Minute)
	var records []record

	i := 0
	for channel := 1; channel <= 6; channel++ {
		for j := 0; j < 60*3; j++ {
			query := fmt.Sprintf(`from(bucket: "znb") |> range(start: %s, stop: %s) |> filter(fn: (r) => r["_measurement"] == "Ampere")   |> filter(fn: (r) => r["channel"] == "%d") |> keep(columns: ["_time", "_value"])`,
				b.Format(Time_Format), e.Format(Time_Format), channel)
			println(query)
			//query := `from(bucket: "znb") |> range(start: 2022-02-11T04:00:00Z, stop: 2022-02-11T04:01:00Z) |> filter(fn: (r) => r["_measurement"] == "Ampere")  |> keep(columns: ["_time", "_value"])`
			//query := `from(bucket: "znb") |> range(start: 2022-02-15T00:40:00Z, stop: 2022-02-15T00:41:00Z) |> filter(fn: (r) => r["_measurement"] == "Ampere") |> filter(fn: (r) => r["channel"] == "1")`

			// get QueryTableResult
			start := time.Now()
			result, err1 := queryAPI.Query(context.Background(), query)
			end := time.Now()
			elapsed := end.Sub(start)
			fmt.Printf("elapsed: %v\n", elapsed)

			if err1 != nil {
				panic(err1)
			}

			// Iterate over query response
			start = time.Now()
			for result.Next() {
				// Notice when group key has changed
				if result.TableChanged() {
					fmt.Printf("table: %s\n", result.TableMetadata().String())
				}
				//records[i].T = result.Record().Time()
				//records[i].V = result.Record().Value().(float64)
				i++
				//// Access data
				records = append(records, record{
					T: result.Record().Time(),
					V: result.Record().Value().(float64),
				})
			}
			end = time.Now()
			elapsed = end.Sub(start)
			fmt.Printf("get value, total:%d elapsed: %v\n", i, elapsed)

			// check for an error
			if result.Err() != nil {
				fmt.Printf("query parsing error: %v\n", result.Err().Error())
			}

			b = b.Add(1 * time.Minute)
			e = e.Add(1 * time.Minute)
		}

		csvFileName := fmt.Sprintf("/home/ubuntu/data/csv/channel-%d-%s.csv", channel, Begin_Time)
		csvFile, err := os.OpenFile(csvFileName, os.O_RDWR|os.O_CREATE, os.ModePerm)
		if err != nil {
			panic(err)
		}

		start := time.Now()
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
