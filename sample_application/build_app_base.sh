#!/bin/bash -e

appname="matmult"
docker="docker"

if [[ -n "$1" ]]; then
    appname="$1"
fi

image=$(echo $appname | tr '[:upper:]' '[:lower:]')
echo "Building Docker image $image."
echo "Using application $appname."
$docker images
$docker build -t $image -f Dockerfile-$appname .
$docker save -o "$appname".tar $image
$docker rmi $image
#$docker load -i "$appname".tar
echo "Done."
