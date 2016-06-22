#!/bin/bash
# Install Docker on Ubuntu

apt-get install -y apt-transport-https ca-certificates bridge-utils
apt-key adv --keyserver hkp://p80.pool.sks-keyservers.net:80 --recv-keys 58118E89F3A912897C070ADBF76221572C52609D
echo "deb https://apt.dockerproject.org/repo ubuntu-precise main" > /etc/apt/sources.list.d/docker.list
apt-get update
apt-get purge lxc-docker
apt-cache policy docker-engine
uname -a
apt-get install -y docker-engine
# service docker start