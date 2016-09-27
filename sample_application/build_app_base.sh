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
echo "FROM debian8sample:1" > Dockerfile
echo "MAINTAINER Peter Bryzgalov" >> Dockerfile
echo "ADD $appname /$appname" >> Dockerfile


$docker images
$docker build -t $image .
$docker save -o "$appname".tar $image
#$docker rmi $image
echo "Done."
