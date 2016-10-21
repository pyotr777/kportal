#!/bin/bash
echo "Making movie out.mp4"
ffmpeg -framerate 5 -i export/cavity_%04d.png -vcodec mpeg4 -r 5 out.mp4