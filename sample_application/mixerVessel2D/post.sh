#!/bin/bash

mfile="movie.mp4"
vserver="http://52.199.55.227:8080/files"

tar -czvf VTK.tar.gz sample/VTK > post.log
ls -l >> post.log
echo "Contacting image server at $vserver" >> post.log
curl -F file=@"VTK.tar.gz" $vserver > $mfile 2>> post.log
ls -l >> post.log
mv $mfile sample/
echo "Sending mail" >> post.log
echo -e "echo \"..\" |  mailx -s \"miservessel movie\" -a $mfile peter@stair.center\n" | mailx -s "mixervessel2D movie" -a sample/movie.mp4 -r kportal.aics.riken@gmail.com peter@stair.center &>> post.log