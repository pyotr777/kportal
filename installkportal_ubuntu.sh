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
	sudo useradd -m kportal || true
	echo "Create directory for kp_server logs"
	KP_HOME=$(sudo su kportal -c 'echo $HOME')
	echo "KP_HOME=$KP_HOME"
	sudo mkdir -p "$KP_HOME/log"
	sudo chown kportal:kportal "$KP_HOME/log"
	sudo chmod 777 "$KP_HOME/log"
	ls -la "$KP_HOME"
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
sudo groupadd docker || true
sudo usermod -aG docker kportal
sudo su kportal -c "docker run hello-world"

if [[ -z $skip_apache ]]; then
	message "Installing Apache with SSL in Docker container"
	mkdir -p /etc/kportal/www/ssl/
	echo "Building new image"
	echo "HOME=$HOME"
	./install_apache2_docker.sh
fi

# Uncomment before live install
# message "Start Apache2"
# sudo su kportal -c ./src/release/start_apache.sh

# Uncomment before live install
# Restarts Docker daemon on port 9555
# sudo ./src/release/start_server.sh

# Uncomment before live install
# message "Start kp_server"
# sudo -E su kportal -c kp_server.sh 9004 -tls
