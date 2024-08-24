package main

import (
	"os"
	"io"
	"math"
	"math/rand"
	"image"
	"image/color"
	"image/gif"
)

var palette = []color.Color{color.White,
			    color.RGBA{0Xff, 0X00, 0X00, 0Xff},
			    color.RGBA{0X00, 0Xff, 0X00, 0Xff},
			    color.RGBA{0X00, 0X00, 0Xff, 0Xff},
		           }

const (
	whiteIndex = 0 // first index into palette
	redIndex   = 1 // second index into palette
	greenIndex = 2 // third index into palette
	blueIndex  = 3 // forth index into palette
)

func main() {
	lissanjous(os.Stdout)
}

func lissanjous(out io.Writer) {
	const (
		cycles  = 5     // number of complete x oscillations
		res     = 0.001 // angular resolution
		size    = 100   // image canvas covers [-size..+size]
		nframes = 64    // number of animated frames
		delay   = 8     // delay between frames in 10ms
	)
	freq  := rand.Float64() * 3.0
	anim  := gif.GIF{LoopCount: nframes}
	phase := 0.0
	for i := 0; i < nframes; i++ {
		rect := image.Rect(0, 0, 2*size+1, 2*size+1)
		img  := image.NewPaletted(rect, palette)
		for t := 0.0; t < cycles * 2 * math.Pi; t += res {
			x := int(size * math.Tan(t) + 0.5)
			y := int(size * math.Sin(t*freq + phase) + 0.5)
			img.SetColorIndex(size + x, size + y, uint8(i % 4))
		}
		phase += 0.1
		anim.Delay = append(anim.Delay, delay)
		anim.Image = append(anim.Image, img)
	}
	gif.EncodeAll(out, &anim)
}
