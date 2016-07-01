#!/bin/bash -e

image="matmult"
docker="docker"

if [[ -n "$1" ]]; then
    image="$1"
fi
echo "Building Docker image."
echo "Using name $image"
$docker ps -a
$docker build -t $image .
$docker save -o "$image".tar $image
$docker rmi $image
#$docker load -i "$image".tar
echo "Done." 
