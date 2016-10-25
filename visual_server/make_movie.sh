#!/bin/bash
echo "Making movie out.mp4"
ffmpeg -framerate 10 -i export/img_%04d.png -vcodec mpeg4 -q:v 2 -r 10 out.mp4