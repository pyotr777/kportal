#!/bin/bash
set -e

# Install K-portal in Ubuntu.
# Apache runs inside Docker container.
# SSL certificates obtained from LetsEncrypt.
# If you don't have fixed DNS address, use self-signed certificates.
# To use self-signed certificates set KP_SELF_CERT environment variable beofre running this script.
# Other environment variables
# KP_WEB_DNS		DNS name of this installation of K-portal.
# KP_WEB_MAIL		E-mail for obtaining SSL certificates. (Used only if KP_SELF_CERT is not set).

#skip_user=1
#skip_kpserver=1
#skip_docker=1
#skip_apache=1
#skip_ssl_cert=1

export TERM="screen-256color"

if [[ -n "$KP_SELF_CERT" ]]; then
	export skip_ssl_cert=1
	echo "Using self-signed SSL certificates."
else
	if [[ -z "$KP_WEB_DNS" ]]; then 
		echo "Need your site domain name for obtaining SSL certificates."
		echo -n "Enter domain name and press [ENTER]: "
		read KP_WEB_DNS
	fi
	if [[ -z "$KP_WEB_MAIL" ]]; then
		echo "Need site administrator's e-mail address."
		echo -n "Enter e-mail address and press [ENTER]: "
		read KP_WEB_MAIL
	fi
fi

if [[ "$HOME" = *travis* ]]; then
	export skip_docker=1
	export skip_tars=1
	export skip_ssl_cert=1
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
chmod 777 "$LOGDIR"
ls -la "$ORG_DIR"

export D_HOST_OPT="-H localhost:9555"

message "0. Installing required packages"
sudo apt-get update > "$LOGDIR/update.log"
sudo apt-get install -y curl libcurl4-openssl-dev libssl-dev bzip2 lbzip2 python python-dev gcc g++ wget make > "$LOGDIR/install.log"
# Test if AUFS is installed
$ORG_DIR/install_aufs.sh

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
		sudo mv -f ./src "$KP_HOME/"
		sudo chown -R kportal:kportal "$KP_HOME/src"
	fi
	echo "Source code in $KP_HOME (src dir)?"
	ls -la "$KP_HOME"
else 
	KP_HOME=$(sudo su kportal -c 'echo $HOME')
	echo "KP_HOME=$KP_HOME"
fi

# Save KP_* environment variables to ENV file
echo "" > $KP_HOME/ENV
for e in $(env | grep "KP_"); do
	echo $e
	echo "export $e" >> $KP_HOME/ENV
done


if [[ -z $skip_kpserver ]]; then 
	message "2. Install kp_server"
	$ORG_DIR/install_kpserver.sh
fi

if [[ -z $skip_docker ]]; then	
	message "3. Install Docker and give permissions to user kportal"
	sudo $ORG_DIR/install_docker.sh
fi

# Add user kportal to docker group
sudo groupadd docker || true
sudo usermod -aG docker kportal || true
sudo su kportal -c "docker images" || true

if [[ -z $skip_apache ]]; then
	message "4. Installing Apache with SSL in Docker container"
	sudo mkdir -p /etc/kportal/www/ssl/
	sudo chown -R kportal:kportal /etc/kportal
	sudo touch /etc/kportal/www/log_kp.txt
	sudo chmod 666 /etc/kportal/www/log_kp.txt
	echo "Building new image"
	echo "HOME=$HOME"
	$ORG_DIR/install_apache2_docker.sh
fi

cd "$KP_HOME"
message "5. Restarting Docker daemon on port 9555"
# start_server.sh should be installed on step 2 to /usr/local/bin.
start_server.sh
if [[ -z $skip_docker ]]; then
	ip a s bridge0 || true
fi
echo "Check docker"
echo "Docker on UNIX soket?"
sudo -E su kportal -c 'docker ps' || true
echo "Docker on 9555?"
sudo -E su kportal -c 'docker $D_HOST_OPT ps -a' || true

message "6. Starting Apache2"
$ORG_DIR/start_apache.sh


message "7. Starting kp_server"
sudo -E su kportal -c 'kp_server.sh 9004 -tls'

# Check that kp_server is still running 
echo "Check that kp_server is running on port 9004"
ps ax | grep "kp_server" | grep 9004


if [[ -z $skip_tars ]]; then
	message "8. Loading Master Image"
	cd "$KP_HOME/src/docker_images"
	sudo -E su kportal -c "docker $D_HOST_OPT load -i master_base_image.tar"

	message "9. Building Base Image"
	cd "$KP_HOME/src/docker_images"
	sudo -E su kportal -c "docker $D_HOST_OPT build --rm -t ubuntu_base ."
	echo "Saving Base Image to tar"
	sudo -E su kportal -c "docker $D_HOST_OPT save -o ubuntu_base.tar ubuntu_base"
	echo "Copying image tar to web site folder"
	sudo -E su kportal -c "mv ubuntu_base.tar /etc/kportal/www/images/"
	sudo chmod 666 "/etc/kportal/www/images/ubuntu_base.tar"
	export IM_ID=$(sudo -E su kportal -c "docker $D_HOST_OPT images | grep ubuntu_base | awk '{ print \$3 }'") || true
	echo "Base Image ID is $IM_ID"
	# Set image ID in configuration file
	sudo sed -r -i 's|<Image\s+id=(.*)/>|<Image id="'$IM_ID'" tag="ubuntu_base"/>|Ig' /etc/kportal/kportal_conf.xml
	cat "/etc/kportal/kportal_conf.xml"
fi

if [[ -z $skip_ssl_cert ]]; then
	message "10. SSL certificates from LetsEncrypt."	
	# Use saved certificates in tar if present
	# Must be in src/ssl/letsencrypt.tar.gz file
	CRT_TAR="$KP_HOME/src/ssl/letsencrypt.tar.gz"
	SSL_DIR="/etc/kportal/ssl"
	if [[ -f "$CRT_TAR" ]]; then
		echo "Found certificates in tar file."
		if [[ ! -d "$SSL_DIR/letsencrypt" || $(ls -1 "$SSL_DIR/letsencrypt" | wc -l) > 1 ]]; then
			# If directory doesn't exist or empty
			echo "Extracting to $SSL_DIR/letsencrypt"
			tar -xzf "$CRT_TAR" -C "$SSL_DIR"
		else
			echo "Installed sertificates found in $SSL_DIR/letsencrypt"
		fi
		# Reconfigure apahce to use LetsEncrypt certificates
		docker $D_HOST_OPT cp reconfigure_apache_ssl.sh apache:/certbot/
		docker $D_HOST_OPT exec apache /certbot/reconfigure_apache_ssl.sh
	else
		# Obtain cerificates from LetsEncrypt and update Apache config file
		docker $D_HOST_OPT cp $KP_HOME/ENV apache:/ENV
		docker $D_HOST_OPT cp install_certbot.sh apache:/certbot/
		docker $D_HOST_OPT cp reconfigure_apache_ssl.sh apache:/certbot/		
		docker $D_HOST_OPT exec apache /certbot/install_certbot.sh
		docker $D_HOST_OPT exec apache /certbot/reconfigure_apache_ssl.sh 
	fi
	echo "Resstarting Apache container with SSL port mapped to 9005."
	$ORG_DIR/start_apache.sh 9005
	if [[ -h "$SSL_DIR/server.crt" && -h "$SSL_DIR/server.key" ]]; then
		echo "kp_server certificates alresdy linked to certificates from LetsEncrypt"
	else
		echo "Creating links to LetsEncrypt certificates for kp_server."
		cd "$SSL_DIR"
		if [[ ! -d "letsencrypt" ]]; then
			echo "Directory with Letsencrypt certificates not found."
			exit 1
		fi
		sudo chmod -R +r letsencrypt
		sudo chmod -R +x letsencrypt/archive
		sudo chmod -R +x letsencrypt/live
		CERT=letsencrypt/archive/$KP_WEB_DNS/cert1.pem
		KEY=letsencrypt/archive/$KP_WEB_DNS/privkey1.pem
		if [[ ! -a "$CERT" ]]; then
			echo "Certificate file not found: $CERT"
			exit 1
		fi
		if [[ ! -a "$KEY" ]]; then
			echo "Key file not found: $KEY"
			exit 1
		fi		
		if [[ -f server.crt ]]; then
			sudo rm -f server.crt
		fi
		if [[ -f server.key ]]; then
			sudo rm -f server.key
		fi
		sudo -E su kportal -c "ln -s $CERT server.crt"
		sudo -E su kportal -c "ln -s $KEY server.key"
		echo "Links to certificates created"
		ls -l
		cd $ORG_DIR
	fi
else
	echo "Restarting Apache continer with SSL on port 9005"
	$ORG_DIR/start_apache.sh 9005
fi

export INSTALL_DIR="$KP_HOME/install"
export BOOSTARCHIVE="boost_1_60_0"
echo "Remove boost archive $INSTALL_DIR/$BOOSTARCHIVE"
sudo rm -rf "$INSTALL_DIR/$BOOSTARCHIVE"
sudo rm -f "$INSTALL_DIR/$BOOSTARCHIVE.tar.bz2"

cd "$ORG_DIR"
# rm ENV

