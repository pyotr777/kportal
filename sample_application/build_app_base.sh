#!/bin/bash -e

appname="matmult"
docker="docker"

if [[ -n "$1" ]]; then
    appname="$1"
fi

image=$(echo $appname | tr '[:upper:]' '[:lower:]')
echo "Building Docker image $image."
echo "Using application $appname."

# Prepare Dockerfile
echo "FROM ubuntu_base" > Dockerfile
echo "MAINTAINER Peter Bryzgalov" >> Dockerfile
echo "ADD $appname /$appname" >> Dockerfile


$docker images
ubuntu_base_id=$(docker images | grep "ubuntu_base" | awk '{print $1}')
if [[ "$ubuntu_base_id" ]]; then
    echo "ubuntu_base image found: $ubuntu_base_id"
    docker rmi $ubuntu_base_id
fi
docker load -i ubuntu_base.tar
echo "New Ubuntu base loaded"
docker images

$docker build -t $image .
$docker save -o "$appname".tar $image
$docker rmi $image
echo "Done."
