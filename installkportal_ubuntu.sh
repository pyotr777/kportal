#!/bin/bash
set -e

# Install K-portal in Ubuntu.
# Apache runs inside Docker container.
# SSL certificates obtained from LetsEncrypt.
# If you don't have fixed DNS address, use self-signed certificates.
# To use self-signed certificates set KP_SELF_CERT environment variable before running this script,
# or set it in env_init file.
# Other environment variables:
# KP_WEB_DNS		DNS name of this installation of K-portal.
# KP_WEB_MAIL		E-mail for obtaining SSL certificates. (Used only if KP_SELF_CERT is not set).

#KP_SKIP_USER=1
#KP_SKIP_KPSERVER=1
#KP_SKIP_DOCKER=1
#KP_SKIP_APACHE=1
#KP_SKIP_SSL_CERT=1
#KP_SKIP_TARS=1

function message {
    echo ""
    echo -en "\033[38;5;70m# \033[m\n"
    echo -en "\033[38;5;70m# $1\033[m\n"
    echo -en "\033[38;5;70m# \033[m\n"
    echo " "
}

message "Installing K-Portal"

ORG_DIR="$(pwd)"
cd "$(dirname $0)"
SOURCE_DIR="$(pwd)"
echo "Using $SOURCE_DIR as working directory."

if [[ -f "$SOURCE_DIR/env_init" ]]; then
	message "Initialising environment with $SOURCE_DIR/env_init file:"
	cat "$SOURCE_DIR/env_init"
	source "$SOURCE_DIR/env_init"
fi

export TERM="screen-256color"

if [[ "$HOME" = *travis* ]]; then
	export KP_SKIP_DOCKER=1
	export KP_SKIP_TARS=1
	export KP_SKIP_SSL_CERT=1
	export KP_SELF_CERT=1
fi

if [[ -n "$KP_SELF_CERT" ]]; then
	export KP_SKIP_SSL_CERT=1
	echo "Using self-signed SSL certificates."
else
	if [[ -z "$KP_WEB_DNS" ]]; then 
		echo "Need your site domain name for obtaining SSL certificates."
		echo -n "Enter domain name and press [ENTER]: "
		read KP_WEB_DNS
		export KP_WEB_DNS
	fi
	if [[ -z "$KP_WEB_MAIL" ]]; then
		echo "Need site administrator's e-mail address."
		echo -n "Enter e-mail address and press [ENTER]: "
		read KP_WEB_MAIL
		export KP_WEB_MAIL
	fi
fi


message "Environment"

env | grep "KP_"
LOGDIR="$SOURCE_DIR/logs"
echo "ORG_DIR=$ORG_DIR"
echo "HOME   =$HOME"
echo "LOGDIR =$LOGDIR"
mkdir -p "$LOGDIR"
chmod 777 "$LOGDIR"
cd "$SOURCE_DIR"
# Enable colors in ls and other(?) commands
echo "T $TERM"
echo "S $SHELL"


export D_HOST_OPT="-H localhost:9555"

message "0. Installing required packages"
sudo apt-get update > "$LOGDIR/update.log"
sudo apt-get install -y curl libcurl4-openssl-dev libssl-dev bzip2 lbzip2 python python-dev gcc g++ wget make bridge-utils > "$LOGDIR/apt_install.log"
# Test if AUFS is installed
$SOURCE_DIR/install_aufs.sh

if [[ -z $KP_SKIP_USER ]]; then 
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
		sudo mv -f ./src "$KP_HOME/"
		sudo chown -R kportal:kportal "$KP_HOME/src"
	fi
	echo "Source code in $KP_HOME (src dir)?"
	ls -la --color=always "$KP_HOME"
else 
	KP_HOME=$(sudo su kportal -c 'echo $HOME')
	echo "KP_HOME=$KP_HOME"
fi

# Save KP_* environment variables to ENV file
sudo touch "$KP_HOME/ENV"
sudo chmod 666 "$KP_HOME/ENV"
echo "" > $KP_HOME/ENV
for e in $(env | grep "KP_"); do
	echo $e
	echo "export $e" >> $KP_HOME/ENV
done


if [[ -z $KP_SKIP_KPSERVER ]]; then 
	message "2. Install kp_server"
	$SOURCE_DIR/install_kpserver.sh
fi

if [[ -z $KP_SKIP_DOCKER ]]; then	
	message "3. Install Docker and give permissions to user kportal"
	sudo $SOURCE_DIR/install_docker.sh
fi

# Add user kportal to docker group
sudo groupadd docker || true
sudo usermod -aG docker kportal || true
sudo su kportal -c "docker images" || true

if [[ -z $KP_SKIP_APACHE ]]; then
	message "4. Installing Apache with SSL in Docker container"
	sudo mkdir -p /etc/kportal/www/ssl/
	sudo chown -R kportal:kportal /etc/kportal
	sudo touch /etc/kportal/www/log_kp.txt
	sudo chmod 666 /etc/kportal/www/log_kp.txt
	echo "Building new image"
	echo "HOME=$HOME"
	$SOURCE_DIR/install_apache2_docker.sh
fi

cd "$KP_HOME"
message "5. Restarting Docker daemon on port 9555"
# start_server.sh should be installed on step 2 to /usr/local/bin.
start_server.sh
if [[ -z $KP_SKIP_DOCKER ]]; then
	ip a s bridge0 || true
fi
echo "Check docker"
echo "Docker on UNIX soket?"
sudo -E su kportal -c 'docker ps' || true
echo "Docker on 9555?"
sudo -E su kportal -c 'docker $D_HOST_OPT ps -a' || true

message "6. Starting Apache2"
if [[ -z $(which start_apache.sh) ]]; then
	sudo cp $SOURCE_DIR/start_apache.sh /usr/local/bin/
fi
start_apache.sh

message "7. Starting kp_server"
sudo -E su kportal -c 'kp_server.sh 9004 -tls'

# Check that kp_server is still running 
echo "Check that kp_server is running on port 9004"
ps ax | grep "kp_server" | grep 9004 || true


if [[ -z $KP_SKIP_SSL_CERT ]]; then
	message "8. SSL certificates from LetsEncrypt."	
	# Use saved certificates in tar if present
	# Must be in src/ssl/letsencrypt.tar.gz file
	CRT_TAR="$KP_HOME/src/ssl/letsencrypt.tar.gz"
	SSL_DIR="/etc/kportal/ssl"
	cd $SOURCE_DIR
	# Copy environment initialisation file
	docker $D_HOST_OPT cp $KP_HOME/ENV apache:/ENV
	if [[ -f "$CRT_TAR" ]]; then
		echo "Found certificates in tar file."
		if [[ ! -d "$SSL_DIR/letsencrypt" || $(ls -1 "$SSL_DIR/letsencrypt" | wc -l) > 1 ]]; then
			# If directory doesn't exist or empty
			echo "Extracting to $SSL_DIR/letsencrypt"
			sudo -E su -p kportal -c "tar -xzf $CRT_TAR -C $SSL_DIR"
		else
			echo "Installed sertificates found in $SSL_DIR/letsencrypt"
		fi
		# Reconfigure apahce to use LetsEncrypt certificates
		docker $D_HOST_OPT cp reconfigure_apache_ssl.sh apache:/certbot/
		docker $D_HOST_OPT exec apache /certbot/reconfigure_apache_ssl.sh
	else
		# Copy script files		
		docker $D_HOST_OPT cp install_certbot.sh apache:/certbot/
		docker $D_HOST_OPT cp reconfigure_apache_ssl.sh apache:/certbot/	
		# Obtain cerificates from LetsEncrypt and update Apache config file	
		docker $D_HOST_OPT exec apache /certbot/install_certbot.sh
		docker $D_HOST_OPT exec apache /certbot/reconfigure_apache_ssl.sh 
	fi
	# Restarting Apache container with SSL port mapped to 9005.
	$SOURCE_DIR/start_apache.sh 9005
	if [[ -h "$SSL_DIR/server.crt" && -h "$SSL_DIR/server.key" && -h "$SSL_DIR/server.cer" ]]; then
		echo "kp_server certificates alresdy linked to certificates from LetsEncrypt"
	else
		echo "Creating links to LetsEncrypt certificates for kp_server."
		cd "$SSL_DIR"
		if [[ ! -d "letsencrypt" ]]; then
			echo "Directory with Letsencrypt certificates not found."
			exit 1
		fi
		ls --color letsencrypt
		sudo chmod -R +r letsencrypt
		sudo chmod -R +x letsencrypt/archive
		sudo chmod -R +x letsencrypt/live
		CERT=$(ls letsencrypt/live/$KP_WEB_DNS/cert*.pem | sed -n 1p)
		if [[ $? -ne 0 ]]; then
			echo "Certificate file not found: $CERT"
			exit 1
		fi
		KEY=$(ls letsencrypt/live/$KP_WEB_DNS/privkey*.pem | sed -n 1p)
		if [[ $? -ne 0 ]]; then
			echo "Key file not found: $KEY"
			exit 1
		fi
		CHAIN=$(ls letsencrypt/live/$KP_WEB_DNS/fullchain*.pem | sed -n 1p)
		if [[ $? -ne 0 ]]; then
			echo "Chain file not found: $CHAIN"
		fi
		echo "Delete self-signed certificates which came from distribution."
		if [[ -f server.crt ]]; then
			sudo rm -f server.crt
		fi
		if [[ -f server.key ]]; then
			sudo rm -f server.key
		fi
		if [[ -f server.cer ]]; then
			sudo rm -f server.cer
		fi
		sudo -E su kportal -c "ln -s $CERT server.crt"
		sudo -E su kportal -c "ln -s $KEY server.key"
		sudo -E su kportal -c "ln -s $CHAIN server.cer"
		echo "Links to certificates created"
		ls -l --color
		cd $ORG_DIR
	fi
else
	# Restarting Apache continer with SSL on port 9005
	$SOURCE_DIR/start_apache.sh 9005
fi

if [[ -z $KP_SKIP_TARS ]]; then
	IM_ID=$(docker $D_HOST_OPT images -q "base_image")
	if [[ -z "$IM_ID" ]]; then
		message "9. Loading Master Image"	
		cd "$KP_HOME/src/docker_images"
		sudo -E su kportal -c "docker $D_HOST_OPT load -i master_base_image.tar"
		echo "Image base_image loaded."
		sudo docker $D_HOST_OPT images
	fi

	message "10. Building Base Image"
	TAR_IMG="/etc/kportal/www/images/ubuntu_base.tar"
	cd "$KP_HOME/src/docker_images"
	IM_ID=$(docker $D_HOST_OPT images -q "ubuntu_base")
	if [[ -z "$IM_ID" ]]; then
		sudo -E su kportal -c "docker $D_HOST_OPT build --rm -f Dockerfile -t ubuntu_base .."
	fi
	if [[ ! -f "ubuntu_base.tar" ]]; then
		echo "Saving Base Image to tar"
		sudo -E su kportal -c "docker $D_HOST_OPT save -o ubuntu_base.tar ubuntu_base"
	fi
	if [[ ! -f "$TAR_IMG" ]]; then
		echo "Copying image tar to web site folder"
		sudo mv ubuntu_base.tar /etc/kportal/www/images/
		sudo chmod 666 "$TAR_IMG"
	else
		echo "Base image already saved to ubuntu_base.tar"
	fi
	IM_ID=$(docker $D_HOST_OPT images -q "ubuntu_base")
	if [[ -z "$IM_ID" ]]; then
		echo "Couldn't get image ID for ubuntu_base" 1>&2
		exit 1
	fi
	echo "Base Image ID is $IM_ID"
	# Set image ID in configuration file
	sudo sed -r -i 's|<Image\s+id=(.*)/>|<Image id="'$IM_ID'" tag="ubuntu_base"/>|Ig' /etc/kportal/kportal_conf.xml
	cat "/etc/kportal/kportal_conf.xml"
fi

export STARTUP_SCRIPT="/usr/local/bin/startup.sh"

str=$(grep "startup" /etc/rc.local) || true
if [[ -z "$str" ]]; then
	message "11. Install startup script"
	sudo cp "$SOURCE_DIR/startup.sh" /usr/local/bin/
	if [[ -f /etc/rc.local ]]; then
		sudo sed -i -r "s#^exit 0#echo \"executing /etc/rc.local\"\n$STARTUP_SCRIPT || exit 1\nexit 0\n#" /etc/rc.local
		echo "Startup script installed."
	else
		echo "Not found /etc/rc.local." 1>&2
		echo "To start K-portal after reboot use $STARTUP_SCRIPT script"
	fi
else 
	echo "Startup script already installed."
	ls -l $STARTUP_SCRIPT
	cat /etc/rc.local
fi

export INSTALL_DIR="$KP_HOME/install"
export BOOSTARCHIVE="boost_1_60_0"
echo "Remove boost archive $INSTALL_DIR/$BOOSTARCHIVE"
sudo rm -rf "$INSTALL_DIR/$BOOSTARCHIVE"
sudo rm -f "$INSTALL_DIR/$BOOSTARCHIVE.tar.bz2"


cd "$ORG_DIR"
sudo rm "$KP_HOME/ENV"

