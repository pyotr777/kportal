#!/bin/bash

# Rename files in target directory to *_n.vtk to sample_*.vtk

if [[ $# == 0 ]]; then
    echo "Need target dir as parameter"
    exit 1
fi

cd $1
pwd

for file in *.vtk; do
    newname=$(echo "$file" | sed -r 's/(^[^_]+)/sample/')
    mv $file $newname
    echo "Renamed $file to $newname"
done
cd -
