#!/bin/bash
#
# Install certbot and get certificated from LetsEncrypt
# E-mail address and DNS address can be provided as command line arguments:
# install_certbot.sh <mail address> <DNS address>

if [[ "$1" ]]; then
	MAIL=$1
else
	echo "Need administrator's e-mail address and your site domain name for obtaining SSL certificates."
	echo -n "Enter e-mail address and press [ENTER]: "
	read MAIL
fi

if [[ "$2" ]]; then 
	DNS=$2
else
	echo -n "Enter domain name and press [ENTER]: "
	read DNS
fi

wget https://dl.eff.org/certbot-auto
chmod +x certbot-auto
./certbot-auto -n --os-packages-only
./certbot-auto certonly -v -n -m $MAIL -d $DNS --apache --renew-by-default --agree-tos 