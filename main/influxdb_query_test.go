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

func TestName(t *testing.T) {
	client := influxdb2.NewClient("http://192.168.1.93:8086", "yvTRbUoJhVR4mI6GLCKwOX1pxczUZCWXiBEjCZ8dEq9W2KQatEPqQhLJxg5sj93c7XVmFTZf89V8f5yH3VVAgw==")
	// always close client at the end
	defer client.Close()

	// Get query client
	queryAPI := client.QueryAPI("znb")

	b, err := time.Parse("2006-01-02T15:04:05Z", "2022-02-16T00:40:00Z")
	if err != nil {
		panic(err)
	}
	e := b.Add(1 * time.Minute)
	var records []record

	i := 0
	for j := 0; j < 60*3; j++ {
		query := fmt.Sprintf(`from(bucket: "znb") |> range(start: %s, stop: %s) |> filter(fn: (r) => r["_measurement"] == "Ampere")   |> filter(fn: (r) => r["channel"] == "1") |> keep(columns: ["_time", "_value"])`,
			b.Format("2006-01-02T15:04:05Z"), e.Format("2006-01-02T15:04:05Z"))
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

	csvFile, err := os.OpenFile("output.csv", os.O_RDWR|os.O_CREATE, os.ModePerm)
	if err != nil {
		panic(err)
	}
	defer csvFile.Close()

	start := time.Now()
	err = gocsv.MarshalFile(&records, csvFile) // Use this to save the CSV back to the file
	if err != nil {
		panic(err)
	}
	end := time.Now()
	elapsed := end.Sub(start)
	fmt.Printf("write csv %d line elapsed: %v\n", len(records), elapsed)
}
