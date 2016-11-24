#!/bin/sh

width=32
dir=""

for file in *.png
do
	filename="${file%.*}"
     convert "${file}" -resize $(($width))x$(($width)) -quality 100 "../${filename}.png"
     convert "${file}" -resize $(($width*2))x$(($width*2)) -quality 100 "../${filename}@2x.png"
     convert "${file}" -resize $(($width*3))x$(($width*3)) -quality 100 "../${filename}@3x.png"
     convert "${file}" -resize $(($width*4))x$(($width*4)) -quality 100 "../${filename}@4x.png"
done