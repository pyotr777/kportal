#!/bin/bash
# Start Apache server in Docker container,
# or restart apache container with SSL on custom host port.
# Argument $1 is used to set custom SSL port number.

DOCKER_HOST="-H 127.0.0.1:9555"
SSL_DIR="/etc/kportal/ssl/"
WWW_DIR="/etc/kportal/www/"
# Name of Docker image that is built from Dockerfile
IMG="apache"
# Name of Docker image used for restarting Apache with SSL on custom port
SSL_IMG="apache_ssl"

if [[ ! -d "$SSL_DIR" ]]; then
	mkdir -p "$SSL_DIR"
	echo "Directory $SSL_DIR created."
fi

SSL=443
if [[ $1 ]]; then
	SSL=$1
fi

# Commit apache container to image SSL_IMG
function create_image {
	img=$(docker $DOCKER_HOST images -q $SSL_IMG)				
	if [[ $img ]]; then
		echo "Image $SSL_IMG already created."
	else
		docker $DOCKER_HOST commit apache $SSL_IMG
	fi
	docker $DOCKER_HOST rm apache
}

# Start container from image SSL_IMG
function restart_container {
	docker $DOCKER_HOST run -d -p 80:80 -p $SSL:443 -v $WWW_DIR:/$WWW_DIR -v $SSL_DIR:$SSL_DIR --name apache $SSL_IMG
	if [[ $? -ne 0 ]]; then
		echo "Couldn't start Docker container with Apache2 from image $SSL_IMG."
		docker $DOCKER_HOST images
		exit 1
	fi
}

docker $DOCKER_HOST images &>/dev/null
if [[ $? -eq 0 ]]; then
	echo "Starting Apache2 container with SSL on port $SSL"
	contid=$(docker $DOCKER_HOST ps -qf name=apache)
	if [[ "$contid" ]]; then
		# Container apache is running
		ssl_port=$(docker $DOCKER_HOST inspect --format='{{(index (index .NetworkSettings.Ports "443/tcp") 0).HostPort}}' apache)
		if [[ "$ssl_port" ]]; then
			if [[ "$ssl_port" == "$SSL" ]]; then
				# SSL port is mapped to what we need
				echo "Apache container is already running with SSL on port $ssl_port."
				exit 0
			else
				# SSL port os not what we need
				echo "Restarting container with SSL on port $SSL"
				docker $DOCKER_HOST stop apache
				create_image
				restart_container
			fi
		else
			# SSL port is not open?
			echo "SSL port is not opened"
			docker $DOCKER_HOST stop apache
			create_image
			restart_container
		fi
	else
		# Container is not running
		contid=$(docker $DOCKER_HOST ps -aqf name=apache)
		if [[ "$contid" ]]; then
			# Container is stopped
			create_image
			restart_container
		else
			# Container not exists
			# Start for the 1st time from image IMG
			docker $DOCKER_HOST run -d -p 80:80 -p $SSL:443 -v /etc/kportal/www:/etc/kportal/www -v $SSL_DIR:/etc/letsencrypt --name apache $IMG
			if [[ $? -ne 0 ]]; then
				echo "Couldn't start Docker container with Apache2 from image $IMG."
				exit 1
			fi
		fi
	fi
else
	# Cannot connect Docker
	echo "Cannot access Docker daemon."
	exit 1
fi

echo "Running containers"
docker $DOCKER_HOST ps
