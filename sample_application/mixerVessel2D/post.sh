#!/bin/bash

echo "Postscript $0 $@" > post.log

if [[ -f "email.source" ]]; then
	source email.source
else
	echo "No email.source found!" >> post.log
	echo "Use default address" >> post.log
	email="peter@stair.center"
fi

mfile="movie.mp4"
vserver="http://52.199.55.227:8080/files"

tar -czvf VTK.tar.gz sample/VTK >> post.log
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
host_name=$(hostname -f)

echo -e "MixerVessel2D simulation\n$job_start\n$job_end \
\n$job_nodes \
\nK COMPUTER NODE NAME\t: $host_name \
\n$job_cpus\n" > mail_body.txt
cat sample/paramaters.txt >> mail_body.txt

mailx -s "mixerVessel2D movie" -a $mfile $email &>> post.log < mail_body.txt
echo "Clean files" >> post.log
rm -rf sample/VTK &>> post.log
rm email.source &>> post.log