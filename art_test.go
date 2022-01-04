package art_go

import (
	"testing"
)

func TestCreate(t *testing.T) {
	art, err := NewArt(0)
	if err != nil {
		print(err)
	} else {
		print("create ok")
		print(art)
	}
}
