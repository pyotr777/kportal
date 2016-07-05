#!/bin/bash
# Install Docker on Ubuntu

docker images &>/dev/null
if [[ $? -eq 0 ]]; then
	echo "Docker daemon already running"
	exit 0
fi
docker -H localhost:9555 images &>/dev/null
if [[ $? -eq 0 ]]; then
	echo "Docker daemon already running on port 9555. Restarting as a service."
	PID=$(ps ax | grep "docker" | grep "daemon" | awk '{ print $1}')
	sudo kill $PID
	sleep 5
	service docker start
	exit 0
fi

apt-get install -y apt-transport-https ca-certificates bridge-utils linux-image-extra-$(uname -r)
apt-key adv --keyserver hkp://p80.pool.sks-keyservers.net:80 --recv-keys 58118E89F3A912897C070ADBF76221572C52609D
echo "deb https://apt.dockerproject.org/repo ubuntu-precise main" > /etc/apt/sources.list.d/docker.list
apt-get update
apt-get purge lxc-docker
apt-cache policy docker-engine
uname -a
apt-get install -y docker-engine
service docker start || true