#!/bin/bash
# Build Docker image with Apache2 with ssl support
pwd
echo "Building apache Docker image"
DOCKER_HOST=""
docker $DOCKER_HOST images &>/dev/null
if [[ "$?" -ne 0 ]];then
	# Couldnt contact docker on Unix socket. Try with port 9555.
	export DOCKER_HOST="-H localhost:9555"
	docker $DOCKER_HOST images &>/dev/null
	if [[ "$?" -ne 0 ]];then
		echo "Couldnt connect to the Docker daemon on default socket and port 9555."
		exit 1
	fi
fi
sudo docker $DOCKER_HOST build --rm -t apache . > /dev/null
sudo docker $DOCKER_HOST images