#!/bin/bash
# Start Apache server in Docker container.
# apachessl image is used. It is built in installkportal_ubuntu.sh script.

DOCKER_HOST="-H 127.0.0.1:9555"

if [[ "$1" && "$1"=="-travis" ]]; then
	# Run under travis
	DOCKER_HOST=""
fi

docker images &>/dev/null
if [[ $? -eq 0 ]]; then
	docker $DOCKER_HOST run -d -p 80:80 -p 9005:443 -v /etc/kportal/www:/etc/kportal/www --name apache apachessl
	if [[ $? -ne 0 ]]; then
		echo "Couldn't start Docker container with Apache2."
		exit 1
	fi
else
	echo "Cannot access Docker daemon."
fi
echo "Running containers"
docker $DOCKER_HOST ps
