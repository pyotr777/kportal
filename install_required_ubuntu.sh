#!/bin/bash
# Install python packages for webint

sudo apt-get update
sudo apt-get install -y python-dev python-pip libxml2-dev libxslt1-dev python-lxml firefox
sudo pip install bottle 
sudo pip install gevent==1.0.1
sudo pip install bottle_websocket
sudo pip install ansi2html
