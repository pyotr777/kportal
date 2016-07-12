#!/bin/bash
#
# Install certbot and get certificated from LetsEncrypt

read -rd '' usage << EOF
Usage:
$0 <mail address> <DNS address>
EOF

if [[ $# < 2 ]]; then
	echo $usage
	exit 1
fi

MAIL=$1
DNS=$2

wget https://dl.eff.org/certbot-auto
chmod +x certbot-auto
./certbot-auto -n --os-packages-only
./certbot-auto certonly -v -n -m $MAIL -d $DNS --apache --renew-by-default --agree-tos 