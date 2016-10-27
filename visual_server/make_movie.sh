#!/bin/bash
echo "Making movie out.mp4"
rm out.mp4
ffmpeg -i export/img_%04d.png -vcodec mpeg4 -q:v 2 out.mp4