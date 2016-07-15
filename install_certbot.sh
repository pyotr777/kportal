#!/bin/bash
#
# Install certbot and get certificated from LetsEncrypt
# Use environment variables for DNS and MAIL address:
# KP_WEB_DNS		DNS name of this installation of K-portal.
# KP_WEB_MAIL		E-mail for obtaining SSL certificates. (Used only if KP_SELF_CERT is not set).
# This variables can also be defined in bash source file /ENV.sh

export TERM="screen-256color"

if [[ -f "/ENV" ]]; then
	source /ENV
fi

if [[ ! "$KP_WEB_DNS" ]]; then
	echo "Envvar KP_WEB_DNS is not set."
	echo -n "Enter domain name (without protocol name: example.com) and press [ENTER]: "
	read KP_WEB_DNS
fi

CERT=/etc/letsencrypt/live/$KP_WEB_DNS/cert.pem
KEY=/etc/letsencrypt/live/$KP_WEB_DNS/privkey.pem

if [[ ! -f "$CERT" || ! -f "$KEY" ]]; then
	echo "Obtaining certificates from LetsEncrypt."
	if [[ ! "$KP_WEB_MAIL" ]]; then
		echo "Need administrator's e-mail address and your site domain name for obtaining SSL certificates."
		echo -n "Enter e-mail address and press [ENTER]: "
		read MAIL
	fi
	echo $KP_WEB_MAIL

	wget https://dl.eff.org/certbot-auto
	chmod +x certbot-auto
	./certbot-auto -n --os-packages-only
	./certbot-auto certonly -v -n -m $KP_WEB_MAIL -d $KP_WEB_DNS --apache --renew-by-default --agree-tos
fi



