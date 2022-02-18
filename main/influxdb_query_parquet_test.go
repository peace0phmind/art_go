package main

import (
	"context"
	"fmt"
	influxdb2 "github.com/influxdata/influxdb-client-go/v2"
	"github.com/rocketlaunchr/dataframe-go"
	"github.com/rocketlaunchr/dataframe-go/exports"
	"github.com/rocketlaunchr/dataframe-go/imports"
	"github.com/xitongsys/parquet-go-source/local"
	"os"
	"testing"
	"time"
)

func TestParquet(t *testing.T) {
	client := influxdb2.NewClient("http://192.168.1.93:8086", "yvTRbUoJhVR4mI6GLCKwOX1pxczUZCWXiBEjCZ8dEq9W2KQatEPqQhLJxg5sj93c7XVmFTZf89V8f5yH3VVAgw==")
	// always close client at the end
	defer client.Close()

	// Get query client
	queryAPI := client.QueryAPI("znb")

	b, err := time.Parse(Time_Format, Begin_Time)
	if err != nil {
		panic(err)
	}
	e := b.Add(1 * time.Minute)

	idx := dataframe.NewSeriesTime("time", nil)
	value := dataframe.NewSeriesFloat64("value", nil)
	df := dataframe.NewDataFrame(idx, value)

	i := 0
	for j := 0; j < 1; j++ {
		query := fmt.Sprintf(`from(bucket: "znb") |> range(start: %s, stop: %s) |> filter(fn: (r) => r["_measurement"] == "Ampere")   |> filter(fn: (r) => r["channel"] == "1") |> keep(columns: ["_time", "_value"])`,
			b.Format(Time_Format), e.Format(Time_Format))
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
			i++
			//// Access data
			df.Append(nil, result.Record().Time(), result.Record().Value().(float64))
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

	parquetFile, err := os.OpenFile("output.parquet", os.O_RDWR|os.O_CREATE, os.ModePerm)
	if err != nil {
		panic(err)
	}
	defer parquetFile.Close()

	start := time.Now()
	exports.ExportToParquet(context.Background(), parquetFile, df)
	end := time.Now()
	elapsed := end.Sub(start)
	fmt.Printf("write parquet %d line elapsed: %v\n", df.NRows(), elapsed)

	fmt.Print(df.Table())
}

func TestImportParquet(t *testing.T) {
	fr, _ := local.NewLocalFileReader("output.parquet")
	defer fr.Close()

	df, _ := imports.LoadFromParquet(context.Background(), fr)
	fmt.Print(df.Table())
}
