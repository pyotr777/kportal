#!/bin/bash
# Start Apache server in Docker container.
# apachessl image is used. It is built in installkportal_ubuntu.sh script.

DOCKER_HOST="-H 127.0.0.1:9555"

if [[ "$1" && "$1"=="-travis" ]]; then
	# Run under travis
	DOCKER_HOST=""
fi

docker images &>/dev/null
if [[ "$?"=="0" ]]; then
	docker $DOCKER_HOST run -d -p 80:80 -p 9005:443 -v /etc/kportal/www:/etc/kportal/www --name apache apachessl
else
	echo "Cannot access Docker daemon."
fi
