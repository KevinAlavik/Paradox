#!/bin/bash

if [ $# -ne 2 ]; then
  echo "Usage: $0 <width> <height>"
  exit 1
fi

width=$1
height=$2

# Echo width and height
echo "$width $height"

# Generate random RGBA values for each pixel
for ((i = 0; i < width * height; i++)); do
  # Generate random values for each channel
  r=$(shuf -i 0-255 -n 1)
  g=$(shuf -i 0-255 -n 1)
  b=$(shuf -i 0-255 -n 1)
  a=$(shuf -i 0-255 -n 1)
  
  echo "$r $g $b $a"
done
