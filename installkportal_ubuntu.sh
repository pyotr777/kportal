#!/bin/bash
set -e

#skip_user=1
#skip_kpserver=1
#skip_docker=1
#skip_apache=1
if [[ "$HOME" = *travis* ]]; then
	export skip_docker=1;
fi

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
message "0. Installing required packages"
sudo apt-get update
sudo apt-get install -y curl libcurl4-openssl-dev libssl-dev bzip2 lbzip2 python python-dev gcc g++ wget make

if [[ -z $skip_user ]]; then 
	message "1. Create user kportal"
	sudo useradd -m kportal || true
	echo "Create directory for kp_server logs"
	KP_HOME=$(sudo su kportal -c 'echo $HOME')
	echo "KP_HOME=$KP_HOME"
	sudo mkdir -p "$KP_HOME/log"
	sudo chown -R kportal:kportal "$KP_HOME/log"
	sudo chmod 777 "$KP_HOME/log"
	# Move source code  to /home/kportal/src
	if [[ ! -d "$KP_HOME/src" ]]; then
		sudo mv ./src "$KP_HOME/"
		sudo chown -R kportal:kportal "$KP_HOME/src"
	fi
	echo "Source code in $KP_HOME (src dir)?"
	ls -la "$KP_HOME"
fi


if [[ -z $skip_kpserver ]]; then 
	message "2. Install kp_server"
	./install_kpserver.sh
fi

if [[ -z $skip_docker ]]; then	
	message "3. Install Docker and give permissions to user kportal"
	sudo ./install_docker.sh
fi

# Add user kportal to docker group
sudo groupadd docker || true
sudo usermod -aG docker kportal || true
sudo su kportal -c "docker run hello-world" || true

if [[ -z $skip_apache ]]; then
	message "4. Installing Apache with SSL in Docker container"
	sudo mkdir -p /etc/kportal/www/ssl/
	sudo chown -R kportal:kportal /etc/kportal
	echo "Building new image"
	echo "HOME=$HOME"
	./install_apache2_docker.sh
fi

cd "$KP_HOME"
message "5. Restarting Docker daemon on port 9555"
src/release/start_server.sh
ip a s
echo "Check docker"
echo "Docker on UNIX soket?"
sudo -E su kportal -c 'docker ps' || true
echo "Docker on 9555?"
sudo -E su kportal -c 'docker -H localhost:9555 ps' || true

message "6. Starting Apache2"
sudo -E su kportal -c "src/release/start_apache.sh"

message "7. Starting kp_server"
sudo -E su kportal -c 'kp_server.sh 9004 -tls'
# Check that kp_server is still running 
echo "Is kp_server still running?"
ps aufx
cd "$ORG_DIR"
