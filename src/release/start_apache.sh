#!/bin/bash
# Start Apache server in Docker container.
# apachessl image is used. It is built in installkportal_ubuntu.sh script.

docker images &>/dev/null
if [[ "$?"=="0" ]]; then
	docker run -d -p 80:80 -p 9005:443 -v /etc/kportal/www:/etc/kportal/www --name apache apachessl
else
	echo "Cannot access Docker daemon."
fi
