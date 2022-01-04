package main

import "github.com/peace0phmind/art_go"

func main() {
	art, err := art_go.NewArt(0)
	if err != nil {
		print(err)
	} else {
		art.Release()
		print("create ok")
	}
}
