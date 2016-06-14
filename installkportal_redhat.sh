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
yum -y install curl curl-devel
yum -y install openssl*
yum -y install bzip2 bzip1g
yum -y install python python-devel
yum -y install gcc-c++

export BOOSTVERSION="1.55.0"
export BOOSTARCHIVE="boost_1_55_0"
message "Installing BOOST $BOOSTVERSION"
if [[ ! -f $BOOSTARCHIVE.tar.bz2 ]]; then
    message "Downloading boost"
    wget http://heanet.dl.sourceforge.net/project/boost/boost/$BOOSTVERSION/$BOOSTARCHIVE.tar.bz2
fi
if [[ ! -d "$BOOSTARCHIVE" ]]; then
    bzip2 -dc "$BOOSTARCHIVE.tar.bz2" | tar xvf -
fi
cd "$BOOSTARCHIVE"
./bootstrap.sh --prefix=/home/kportal/usr
./b2 install
cd /home/travis/build/pyotr777/kportal/src && make && sudo make install