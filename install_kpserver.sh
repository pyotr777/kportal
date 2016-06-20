#!/bin/bash
# Install Boost library, build kp_server, install it.

ORG_DIR = $(pwd)
echo "Came from $ORG_DIR"
HOME_DIR = $(sudo -u kportal echo $HOME)

mkdir -p /install
cd /install
export BOOSTVERSION="1.60.0"
export BOOSTARCHIVE="boost_1_60_0"
echo "Installing BOOST $BOOSTVERSION"
if [[ ! -f $BOOSTARCHIVE.tar.bz2 ]]; then
    echo "Downloading boost"
    wget http://heanet.dl.sourceforge.net/project/boost/boost/$BOOSTVERSION/$BOOSTARCHIVE.tar.bz2
fi
if [[ ! -d "$BOOSTARCHIVE" ]]; then
    bzip2 -dc "$BOOSTARCHIVE.tar.bz2" | tar xf -
fi
cd "$BOOSTARCHIVE"
sudo ./bootstrap.sh --prefix=$HOME_DIR/usr > /install/boostinstall.log
sudo ./b2 install >> /install/boostinstall.log
echo "Building and installing kp_server"
cd $ORG_DIR/src 
export CPLUS_INCLUDE_PATH=$HOME_DIR/usr/include/
make 
sudo make install