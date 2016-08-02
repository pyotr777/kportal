#!/bin/bash
# Start Apache server in Docker container,
# or restart apache container with SSL on custom host port.
# Argument $1 is used to set custom SSL port number.

DOCKER_COMMAND="docker -H 127.0.0.1:9555"
$DOCKER_COMMAND images &>/dev/null
if [[ "$?" != 0 ]]; then
	export DOCKER_COMMAND="sudo docker"
fi
$DOCKER_COMMAND images &>/dev/null
if [[ "$?" != 0 ]]; then
	echo "Cannot connect to docker daemon with sudo docker and docker -H localhost:9555."
	exit 1
fi



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
	img=$($DOCKER_COMMAND images -q $SSL_IMG)				
	if [[ $img ]]; then
		echo "Image $SSL_IMG already created."
	else
		$DOCKER_COMMAND commit apache $SSL_IMG
	fi
	$DOCKER_COMMAND rm apache
}

# Start container from image SSL_IMG
function restart_container {
	$DOCKER_COMMAND run -d -p 80:80 -p $SSL:443 -v $WWW_DIR:$WWW_DIR -v $SSL_DIR:$SSL_DIR --name apache $SSL_IMG
	if [[ $? -ne 0 ]]; then
		echo "Couldn't start Docker container with Apache2 from image $SSL_IMG."
		$DOCKER_COMMAND images
		exit 1
	fi
}

$DOCKER_COMMAND images &>/dev/null
if [[ $? -eq 0 ]]; then
	# Check that container just stopped. Then start it and perform port tests.
	# Container is not running
	contid=$($DOCKER_COMMAND ps -aqf name=apache)
	if [[ "$contid" ]]; then
		# Container is stopped
		$DOCKER_COMMAND start apache
		sleep 1
	fi

	echo "Check that Apache container is running with SSL on port $SSL"
	contid=$($DOCKER_COMMAND ps -qf name=apache)
	if [[ "$contid" ]]; then
		# Container apache is running
		ssl_port=$($DOCKER_COMMAND inspect --format='{{(index (index .NetworkSettings.Ports "443/tcp") 0).HostPort}}' apache)
		if [[ "$ssl_port" ]]; then
			if [[ "$ssl_port" == "$SSL" ]]; then
				# SSL port is mapped to what we need
				echo "Apache container is already running with SSL on port $ssl_port."
				exit 0
			else
				# SSL port os not what we need
				echo "Restarting container with SSL on port $SSL"
				$DOCKER_COMMAND stop apache
				create_image
				restart_container
			fi
		else
			# SSL port is not open?
			echo "SSL port is not opened"
			$DOCKER_COMMAND stop apache
			create_image
			restart_container
		fi
	else
		# Container is not running
		contid=$($DOCKER_COMMAND ps -aqf name=apache)
		if [[ "$contid" ]]; then
			# Container is stopped
			create_image
			restart_container
		else
			# Container not exists
			# Start for the 1st time from image IMG
			$DOCKER_COMMAND run -d -p 80:80 -p $SSL:443 -v $WWW_DIR:$WWW_DIR -v $SSL_DIR:$SSL_DIR --name apache $IMG
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
sleep 2
echo "Running containers"
$DOCKER_COMMAND ps
contid=$($DOCKER_COMMAND ps -qf name=apache)
if [[ -z "$contid" ]]; then
	# Apache container is not running
	echo "Restarting Apache container"
	$DOCKER_COMMAND start apache
	sleep 2
	contid=$($DOCKER_COMMAND ps -qf name=apache)
	if [[ -z "$contid" ]]; then
		echo "Apache container failed to start"
		$DOCKER_COMMAND logs apache
		exit 1
	else 
		echo "Apache container started with ID $contid"
		$DOCKER_COMMAND ps
	fi
fi
