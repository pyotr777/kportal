#!/bin/bash
echo "Removing dir sample/"
if [[ -d "sample" ]]; then
    rm -rf sample
fi
echo "Making movie out.mp4"
if [[ -f "out.mp4" ]]; then
    rm out.mp4
fi
ffmpeg -framerate 8 -i export/img_%04d.png -vcodec mpeg4 -q:v 2 out.mp4
echo "Removing dir export"
rm -rf export