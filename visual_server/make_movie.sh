#!/bin/bash
echo "Removing dir sample/"
if [[ -d "sample" ]]; then
    rm -rf sample
fi
echo "Making movie out.mp4"
if [[ -f "out.mp4" ]]; then
    rm out.mp4
fi
ffmpeg -i export/img_%04d.png -vcodec mpeg4 -q:v 2 out.mp4