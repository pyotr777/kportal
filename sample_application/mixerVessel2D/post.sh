#!/bin/bash

echo "Postscript $0 $@" > post.log
echo "In $(pwd)" >> post.log
find . >> post.log

if [[ -f "sample/email.source" ]]; then
	source sample/email.source
    echo "Source email.source:" >> post.log
    cat sample/email.source >> post.log
    echo "E-mail : $email" >> post.log
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

# Get job information
inf_file=$(ls -1 job.i* | head -1)
job_start=$(cat $inf_file | grep -i "job start date")
job_end=$(cat $inf_file | grep -i "job end date")
job_nodes=$(cat $inf_file | grep -i "NODE NUM (REQUIRE)")
job_cpus=$(cat $inf_file | grep -i "CPU NUM (REQUIRE)")
host_name=$(cat sample/hostname.txt)

echo "hostname: $host_name" >> post.log

echo -e "MixerVessel2D simulation\n \
\nK COMPUTER NODE NAME : $host_name \
\n$job_nodes \
\n$job_cpus \
\n $job_start\n$job_end \
\n" > mail_body.txt
cat sample/parameters.txt >> mail_body.txt
echo "Sending mail to $email" >> post.log

mailx -s "mixerVessel2D movie" -a $mfile -r kportal.aics.riken@gmail.com $email &>> post.log < mail_body.txt
echo "move movie file to sample directory" >> post.log
mv $mfile sample/
echo "Clean files" >> post.log
rm -rf sample/VTK &>> post.log
#rm email.source &>> post.log