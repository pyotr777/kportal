#!/bin/bash

tar -czvf VTK.tar.gz sample/VTK > post.log
ls -l >> post.log
echo "Contacting image server at http://52.199.55.227:8080/files" >> post.log
curl -F file=@"VTK.tar.gz" http://52.199.55.227:8080/files > movie.mp4 2>> post.log
echo "Movie movie.mp4 received." >> post.log
ls -l >> post.log