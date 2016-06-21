#!/bin/bash
# Install Boost library, build kp_server, install it.

ORG_DIR=$(pwd)
echo "Came from $ORG_DIR"
HOME_DIR=$(sudo su kportal -c 'echo $HOME')
echo "HOME_DIR=$HOME_DIR"
INSTALL_DIR="$HOME_DIR/install"
mkdir -p $INSTALL_DIR
cd $INSTALL_DIR
export BOOSTVERSION="1.60.0"
export BOOSTARCHIVE="boost_1_60_0"
echo "Installing BOOST $BOOSTVERSION into $HOME_DIR/usr"
if [[ ! -f $BOOSTARCHIVE.tar.bz2 ]]; then
    echo "Downloading boost"
    wget http://heanet.dl.sourceforge.net/project/boost/boost/$BOOSTVERSION/$BOOSTARCHIVE.tar.bz2
fi
if [[ ! -d "$BOOSTARCHIVE" ]]; then
    bzip2 -dc "$BOOSTARCHIVE.tar.bz2" | tar xf -
fi
cd "$BOOSTARCHIVE"
echo "Building boost library"
sudo ./bootstrap.sh --prefix=$HOME_DIR/usr > $INSTALL_DIR/boostinstall.log
echo "Installing boost library. Logs are in $INSTALL_DIR/boostinstall.log."
sudo ./b2 install >> $INSTALL_DIR/boostinstall.log
echo "Boost installed into $HOME_DIR/usr?"
ls -l $HOME_DIR/usr/include/
ls -l $HOME_DIR/usr/lib/
echo "Building and installing kp_server"
cd $ORG_DIR/src 
export CPLUS_INCLUDE_PATH=$HOME_DIR/usr/include/
echo $CPLUS_INCLUDE_PATH
make
sudo make install
echo "Check kp_server installation"
echo "PATH=$PATH"
which kp_server
ls -l $(which kp_server)
cd $ORG_DIR