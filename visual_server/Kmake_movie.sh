#!/bin/bash -e

dirs="mixerVessel2D"
dir="sample"
mfile="movie.mp4"
vserver="http://52.199.55.227:8080/files"

if [[ -d $dirs ]]; then
	echo "Moving $dirs to $dir".
	mv $dirs $dir
fi

if [[ ! -f "$dir/VTK.tar.gz" ]]; then
	tar -czvf $dir/VTK.tar.gz $dir/VTK
fi

echo "Contacting image server at $vserver"
curl -F file=@"$dir/VTK.tar.gz" $vserver > $dir/$mfile
ls -l
echo "Sending mail"

mailx -s "mixerVessel2D movie" -a $dir/$mfile peter@stair.center < parameters.txt

