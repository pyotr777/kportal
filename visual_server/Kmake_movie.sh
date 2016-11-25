#!/bin/bash -e

dirs="mixerVessel2D"
dir="sample"
mfile="movie.mp4"
vserver="http://52.199.55.227:8080/files"

if [[ -f "$dir/email.source" ]]; then
	source $dir/email.source
    echo "Source email.source:"
    cat $dir/email.source
    echo "E-mail : $email"
else
	echo "No email.source found!"
	echo "Use default address"
	email="peter@stair.center"
fi

if [[ ! -f "VTK.tar.gz" ]]; then
	if [[ -d "$dir/VTK" ]]; then
		tar -czvf VTK.tar.gz $dir/VTK
	else
		echo "No file VTK.tar.gz no directory $dir/VTK found. Exiting."
		exit 1
	fi
else
	echo "Tar file VTK.tar.gz found."
fi

ls -l
echo "Contacting image server at $vserver"
curl -F file=@"VTK.tar.gz" $vserver > $mfile
#rm VTK.tar.gz
ls -l

# Get job information
inf_file=$(ls -1 job.i* | head -1)
job_start=$(cat $inf_file | grep -i "job start date")
job_end=$(cat $inf_file | grep -i "job end date")
job_nodes=$(cat $inf_file | grep -i "NODE NUM (REQUIRE)")
job_cpus=$(cat $inf_file | grep -i "CPU NUM (REQUIRE)")
host_name=$(cat $dir/hostname.txt)

echo "hostname: $host_name"

echo -e "MixerVessel2D simulation\n \
\nK COMPUTER NODE NAME : $host_name \
\n$job_nodes \
\n$job_cpus \
\n $job_start\n$job_end \
\n" > mail_body.txt
cat $dir/parameters.txt >> mail_body.txt
echo "Sending mail to $email"

mailx -s "mixerVessel2D movie" -a $mfile -r kportal.aics.riken@gmail.com $email &>> post.log < mail_body.txt
echo "move movie file to $dir directory"
mv $mfile $dir/
echo "Clean files"
rm -rf $dir/VTK