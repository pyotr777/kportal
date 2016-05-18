#!/bin/bash

set -e

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
apt-get update
apt-get install -y curl libcurl4-openssl-dev libssl-dev bzip2 lbzip2 python python-dev gcc g++ wget

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
