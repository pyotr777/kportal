#!/bin/bash

mfile="movie.mp4"
vserver="http://52.199.55.227:8080/files"

tar -czvf VTK.tar.gz sample/VTK > post.log
ls -l >> post.log
echo "Contacting image server at $vserver" >> post.log
curl -F file=@"VTK.tar.gz" $vserver > $mfile 2>> post.log
rm VTK.tar.gz
ls -l >> post.log
echo "Sending mail" >> post.log
# Get job information
inf_file=$(ls -1 job.i* | head -1)
job_start=$(cat $inf_file | grep -i "job start date")
job_end=$(cat $inf_file | grep -i "job end date")
job_nodes=$(cat $inf_file | grep -i "NODE NUM (REQUIRE)")
job_cpus=$(cat $inf_file | grep -i "CPU NUM (REQUIRE)")

echo -e "MixerVessle2D simulation\n$job_start\n$job_end\n$job_nodes\n$job_cpus\n" | mailx -s "mixerVessel2D movie" -a $mfile -r kportal.aics.riken@gmail.com peter@stair.center &>> post.log