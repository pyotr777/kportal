#!/bin/bash
# Install Apache2 web server on Ubuntu OS.
# 

ROOT_DIR="/etc/kportal/www"
HOME_DIR="/home/travis/build/pyotr777"

echo "Create DocumentRoot and SSL folders"
mkdir -p "$ROOT_DIR/ssl/"


apt-get install -y apache2 apache2-doc apache2-utils
a2enmod ssl
source /etc/apache2/envvars 
apache_ver=$(apache2 -v | grep -ioP "apache/\K\d\.\d")
echo "Apache version: $apache_ver"
if [[ "$apache_ver" == "2.2" ]]; then
    echo "Apache2.2 configuration scenario"
    a2ensite default-ssl
    echo "Copy configuration files"
    mv /etc/apache2/apache2.conf /etc/apache2/apache2-org.conf
    cp /kportal/settings/ubuntu12/apache2.conf /etc/apache2/apache2.conf
    mv /etc/apache2/ports.conf /etc/apache2/ports-org.conf
    cp /home/travis/build/pyotr777/kportal/settings/ubuntu12/ports.conf /etc/apache2/ports.conf
    mv /etc/apache2/sites-available/default /etc/apache2/sites-available/default-org
    cp /home/travis/build/pyotr777/kportal/settings/ubuntu12/default /etc/apache2/sites-available/default
    mv /etc/apache2/sites-available/default-ssl /etc/apache2/sites-available/default-ssl-org
    cp /home/travis/build/pyotr777/kportal/settings/ubuntu12/default-ssl /etc/apache2/sites-available/default-ssl
else
    echo "Original configuration files"
    find /etc/apache2/ -name "*.conf" | xargs ls -o
    echo "/etc/apache2/apache2.conf"
    cat /etc/apache2/apache2.conf
    echo "------------------"
    echo ""
    echo "/etc/apache2/ports.conf"
    cat /etc/apache2/ports.conf
    echo "------------------"
    echo ""
    echo "Copy configuration files"
    mv /etc/apache2/apache2.conf /etc/apache2/apache2-default.conf
    cp /home/travis/build/pyotr777/kportal/settings/ubuntu14/apache2.conf /etc/apache2/apache2.conf
    mv /etc/apache2/ports.conf /etc/apache2/ports-default.conf
    cp /home/travis/build/pyotr777/kportal/settings/ubuntu14/ports.conf /etc/apache2/ports.conf
fi

echo "Generate SSL certificate and key"
export country="JP"
export state="Chiba"
export locality="Narashino"
export organization="stair.lab"
export commonname="*.amazonaws.com"
export email="kportal.aics.riken@gmail.com"
openssl req -x509 -nodes -days 1095 -newkey rsa:2048 -out /etc/kportal/www/ssl/server.crt \
 -keyout /etc/kportal/www/ssl/server.key \
 -subj "/C=$country/ST=$state/L=$locality/O=$organization/OU=$organizationalunit/CN=$commonname/emailAddress=$email"

echo "Restart Apache2 and check if it works"
service apache2 restart
ps aux | grep apache
# curl localhost:80 
curl localhost:9005