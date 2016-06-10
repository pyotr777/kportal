#!/bin/bash
#set -e

skip_kpserver=1

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
# apt-get update
apt-get install -y curl libcurl4-openssl-dev libssl-dev bzip2 lbzip2 python python-dev gcc g++ wget curl

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
fi

message "Install Apache"
apt-get install -y apache2 apache2-doc apache2-utils
a2enmod ssl
sudo a2ensite default-ssl
message "Copy configuration files"
find /etc/apache2/ -name "*.conf"
mv /etc/apache2/mods-available/ssl.conf /etc/apache2/mods-available/ssl-default.conf
cp ./settings/ssl.conf /etc/apache2/mods-available/ssl.conf
mv /etc/apache2/httpd.conf /etc/apache2/httpd-default.conf
cp ./settings/httpd.conf /etc/apache2/httpd.conf
message "Restart Apache2 and check if it works"
service apache2 restart
ps aux | grep httpd
ps aux | grep apache
curl localhost:80
curl localhost:9005


