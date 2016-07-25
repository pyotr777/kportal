#!/bin/bash
# Install python packages for webint
# and start webint for installing K-portal.

REMOTE_PORT=8080

sudo apt-get update
sudo apt-get install -y python-dev python-pip libxml2-dev libxslt1-dev python-lxml
sudo pip install bottle 
sudo pip install gevent==1.0.1
sudo pip install bottle_websocket

echo "Open the following address in your Internet browser to continue installation:"
echo "http://$1:$REMOTE_PORT"

cd $HOME/kportal/webint
./webint.py



