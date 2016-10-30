#!/bin/bash -e

dirs="mixerVessel2D"
dir="sample"
mfile="movie.mp4"
vserver="http://52.199.55.227:8080/files"

if [[ -d $dirs ]]; then
	echo "Moving $dirs to $dir".
	mv $dirs $dir
fi

tar -czvf $dir/VTK.tar.gz $dir/VTK

echo "Contacting image server at $vserver"
curl -F file=@"$dir/VTK.tar.gz" $vserver > $dir/$mfile
rm $dir/VTK.tar.gz
ls -l 
echo "Sending mail"

echo -e "MixerVessle2D simulation" | mailx -s "mixerVessel2D movie" -a $dir/$mfile peter@stair.center

