#!/bin/bash
#set -e

skip_kpserver=1
skip_apache=1
skip_docker=1
apache_in_container=1

function message {
    echo ""
    echo -en "\033[38;5;70m# \033[m\n"
    echo -en "\033[38;5;70m# $1\033[m\n"
    echo -en "\033[38;5;70m# \033[m\n"
    echo " "
}

message "Installing K-Portal"
mkdir -p /install
cd /install
message "Install required packages"
apt-get update
apt-get install -y curl libcurl4-openssl-dev libssl-dev bzip2 lbzip2 python python-dev gcc g++ wget

if [[ -z $skip_kpserver ]]; then 
	message "Installing K-Portal"
	export BOOSTVERSION="1.60.0"
	export BOOSTARCHIVE="boost_1_60_0"
	message "Installing BOOST $BOOSTVERSION"
	if [[ ! -f $BOOSTARCHIVE.tar.bz2 ]]; then
	    message "Downloading boost"
	    wget http://heanet.dl.sourceforge.net/project/boost/boost/$BOOSTVERSION/$BOOSTARCHIVE.tar.bz2
	fi
	if [[ ! -d "$BOOSTARCHIVE" ]]; then
	    bzip2 -dc "$BOOSTARCHIVE.tar.bz2" | tar xf -
	fi
	cd "$BOOSTARCHIVE"
	./bootstrap.sh --prefix=/home/kportal/usr
	./b2 install
	cd /home/travis/build/pyotr777/kportal/src && make && sudo make install
fi

if [[ -z $skip_docker ]]; then
	message "Install Docker"
	apt-get install -y apt-transport-https ca-certificates
	apt-key adv --keyserver hkp://p80.pool.sks-keyservers.net:80 --recv-keys 58118E89F3A912897C070ADBF76221572C52609D
	echo "deb https://apt.dockerproject.org/repo ubuntu-precise main" > /etc/apt/sources.list.d/docker.list
	apt-get update
	apt-get purge lxc-docker
	apt-cache policy docker-engine
	uname -a
	apt-get install -y docker-engine
	service docker start
	docker run hello-world
	groupadd docker
	usermod -aG docker kportal
fi

if [[ -z $skip_apache ]]; then
	echo "Create DocumentRoot and SSL folders"
	mkdir -p /etc/kportal/www/ssl/

	message "Install Apache"
	apt-get install -y apache2 apache2-doc apache2-utils
	a2enmod ssl
	source /etc/apache2/envvars 
	apache_ver=$(apache2 -v | grep -ioP "apache/\K\d\.\d")
	echo "Apache version: $apache_ver"
	if [[ "$apache_ver" == "2.2" ]]; then
		echo "Apache2.2 configuration scenario"
		a2ensite default-ssl
		message "Copy configuration files"
		mv /etc/apache2/apache2.conf /etc/apache2/apache2-org.conf
		cp /home/travis/build/pyotr777/kportal/settings/ubuntu12/apache2.conf /etc/apache2/apache2.conf
		mv /etc/apache2/ports.conf /etc/apache2/ports-org.conf
		cp /home/travis/build/pyotr777/kportal/settings/ubuntu12/ports.conf /etc/apache2/ports.conf
		mv /etc/apache2/sites-available/default /etc/apache2/sites-available/default-org
		cp /home/travis/build/pyotr777/kportal/settings/ubuntu12/default /etc/apache2/sites-available/default
		mv /etc/apache2/sites-available/default-ssl /etc/apache2/sites-available/default-ssl-org
		cp /home/travis/build/pyotr777/kportal/settings/ubuntu12/default-ssl /etc/apache2/sites-available/default-ssl
	else
		message "Original configuration files"
		find /etc/apache2/ -name "*.conf" | xargs ls -o
		echo "/etc/apache2/apache2.conf"
		cat /etc/apache2/apache2.conf
		echo "------------------"
		echo ""
		echo "/etc/apache2/ports.conf"
		cat /etc/apache2/ports.conf
		echo "------------------"
		echo ""
		message "Copy configuration files"
		mv /etc/apache2/apache2.conf /etc/apache2/apache2-default.conf
		cp /home/travis/build/pyotr777/kportal/settings/ubuntu14/apache2.conf /etc/apache2/apache2.conf
		mv /etc/apache2/ports.conf /etc/apache2/ports-default.conf
		cp /home/travis/build/pyotr777/kportal/settings/ubuntu14/ports.conf /etc/apache2/ports.conf
	fi

	message "Generate SSL certificate and key"
	export country="JP"
	export state="Chiba"
	export locality="Narashino"
	export organization="stair.lab"
	export commonname="*.amazonaws.com"
	export email="kportal.aics.riken@gmail.com"
	openssl req -x509 -nodes -days 1095 -newkey rsa:2048 -out /etc/kportal/www/ssl/server.crt \
	 -keyout /etc/kportal/www/ssl/server.key \
	 -subj "/C=$country/ST=$state/L=$locality/O=$organization/OU=$organizationalunit/CN=$commonname/emailAddress=$email"

	message "Restart Apache2 and check if it works"
	service apache2 restart
	ps aux | grep apache
	# curl localhost:80 
	curl localhost:9005

elif [[ -n $apache_in_container ]]; then 
	message "Install Apache with SSL in Docker container"
	cd /home/travis/build/pyotr777/kportal/
	pwd
	ls -l
	docker images
	docker build --rm -t apachessl .
	docker images
	docker run -d -p 80:80 -p 9005:443 -v /etc/kportal/www:/etc/kportal/www --name apache apachessl 
	docker ps
fi
