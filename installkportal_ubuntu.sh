#!/bin/bash
set -e

#skip_user=1
#skip_kpserver=1
skip_docker=1
#skip_apache=1

function message {
    echo ""
    echo -en "\033[38;5;70m# \033[m\n"
    echo -en "\033[38;5;70m# $1\033[m\n"
    echo -en "\033[38;5;70m# \033[m\n"
    echo " "
}

message "Installing K-Portal"
ORG_DIR=$(pwd)
echo "ORG_DIR=$ORG_DIR"
echo "HOME   =$HOME"
message "Installing required packages"
sudo apt-get update
sudo apt-get install -y curl libcurl4-openssl-dev libssl-dev bzip2 lbzip2 python python-dev gcc g++ wget

if [[ -z $skip_user ]]; then 
	message "Create user kportal"
	sudo useradd -m kportal
fi


if [[ -z $skip_kpserver ]]; then 
	message "Install K-Portal"
	./install_kpserver.sh
fi

if [[ -z $skip_docker ]]; then	
	message "Install Docker and give permissions to user kportal"
	sudo ./install_docker.sh
fi

# Add user kportal to docker group
groupadd docker || true
usermod -aG docker kportal
sudo -u kportal docker run hello-world

if [[ -z $skip_apache ]]; then
	message "Installing Apache with SSL in Docker container"
	mkdir -p /etc/kportal/www/ssl/
	echo "Building new image"
	echo "HOME=$HOME"
	./install_apache2_docker.sh
fi

message "Start Apache2"
# Remove -travis before live install
sudo -u kportal ./src/release/start_apache.sh -travis 

message "Start kp_server"
sudo -u kportal kp_server.sh 9004 -tls

# Uncomment before live install
# 	./src/release/start_server.sh