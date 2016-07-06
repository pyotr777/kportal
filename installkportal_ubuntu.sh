#!/bin/bash
set -e

#skip_user=1
#skip_kpserver=1
#skip_docker=1
#skip_apache=1
if [[ "$HOME" = *travis* ]]; then
	export skip_docker=1;
	export skip_tars=1;
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
LOGDIR="$ORG_DIR/logs"
echo "ORG_DIR=$ORG_DIR"
echo "HOME   =$HOME"
echo "LOGDIR =$LOGDIR"
mkdir -p "$LOGDIR"

message "0. Installing required packages"
sudo apt-get update > "$LOGDIR/update.log"
sudo apt-get install -y curl libcurl4-openssl-dev libssl-dev bzip2 lbzip2 python python-dev gcc g++ wget make > "$LOGDIR/install.log"
# Test if AUFS is installed
./install_aufs.sh

if [[ -z $skip_user ]]; then 
	message "1. Create user kportal"
	sudo useradd -m kportal || true
	echo "Create directory for kp_server logs"
	KP_HOME=$(sudo su kportal -c 'echo $HOME')
	echo "KP_HOME=$KP_HOME"
	sudo mkdir -p "$KP_HOME/log"
	sudo chown -R kportal:kportal "$KP_HOME/log"
	sudo chmod 777 "$KP_HOME/log"
	sudo -E su kportal -c "mkdir -p $KP_HOME/.ssh/kportal"
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
sudo su kportal -c "docker images" || true

if [[ -z $skip_apache ]]; then
	message "4. Installing Apache with SSL in Docker container"
	sudo mkdir -p /etc/kportal/www/ssl/
	sudo chown -R kportal:kportal /etc/kportal
	touch /etc/kportal/www/log_kp.txt
	sudo chmod 666 /etc/kportal/www/log_kp.txt
	echo "Building new image"
	echo "HOME=$HOME"
	./install_apache2_docker.sh
fi

cd "$KP_HOME"
message "5. Restarting Docker daemon on port 9555"
src/release/start_server.sh
if [[ -z $skip_docker ]]; then
	ip a s bridge0 || true
fi
echo "Check docker"
echo "Docker on UNIX soket?"
sudo -E su kportal -c 'docker ps' || true
echo "Docker on 9555?"
sudo -E su kportal -c 'docker -H localhost:9555 ps' || true

message "6. Starting Apache2"
sudo -E su kportal -c "$KP_HOME/src/release/start_apache.sh"

message "7. Starting kp_server"
sudo -E su kportal -c 'kp_server.sh 9004 -tls'

# Check that kp_server is still running 
echo "Check that kp_server is running on port 9004"
ps ax | grep "kp_server" | grep 9004


if [[ -z $skip_tars ]]; then
	message "8. Loading Master Image"
	cd "$KP_HOME/src/docker_images"
	sudo -E su kportal -c "docker -H localhost:9555 load -i master_base_image.tar"

	message "9. Building Base Image"
	cd "$KP_HOME/src/docker_images"
	sudo -E su kportal -c "docker -H localhost:9555 build --rm -t ubuntu_base ."
	echo "Saving Base Image to tar"
	sudo -E su kportal -c "docker -H localhost:9555 save -o ubuntu_base.tar ubuntu_base"
	echo "Copying image tar to web site folder"
	sudo -E su kportal -c "mv ubuntu_base.tar /etc/kportal/www/images/"
	sudo chmod 666 "/etc/kportal/www/images/ubuntu_base.tar"
	export IM_ID=$(sudo -E su kportal -c "docker -H localhost:9555 images | grep ubuntu_base | awk '{ print \$3 }'") || true
	echo "Base Image ID is $IM_ID"
	# Set image ID in configuration file
	sudo sed -r -i 's|<Image\s+id=(.*)/>|<Image id="'$IM_ID'" tag="ubuntu_base"/>|Ig' /etc/kportal/kportal_conf.xml
	cat "/etc/kportal/kportal_conf.xml"
fi

export INSTALL_DIR="$KP_HOME/install"
export BOOSTARCHIVE="boost_1_60_0"
echo "Remove boost archive $INSTALL_DIR/$BOOSTARCHIVE"
sudo rm -rf "$INSTALL_DIR/$BOOSTARCHIVE"
sudo rm -f "$INSTALL_DIR/$BOOSTARCHIVE.tar.bz2"

cd "$ORG_DIR"

