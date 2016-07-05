#!/bin/bash
# Install Boost library, build kp_server, install it.

ORG_DIR=$(pwd)
echo "Came from $ORG_DIR"
KP_HOME=$(sudo su kportal -c 'echo $HOME')
echo "KP_HOME=$KP_HOME"
export INSTALL_DIR="$KP_HOME/install"
sudo -E su -p kportal -c 'echo "INSTALL_DIR=$INSTALL_DIR"'
sudo -E su -p kportal -c 'mkdir -p "$INSTALL_DIR"'
sudo chown -R kportal:kportal "$INSTALL_DIR"
echo "Created $INSTALL_DIR:"
ls -la $INSTALL_DIR
cd "$INSTALL_DIR"
echo "Saving boost in $(pwd)."
export BOOSTVERSION="1.60.0"
export BOOSTARCHIVE="boost_1_60_0"
echo "Installing BOOST $BOOSTVERSION into $KP_HOME/usr"
if [[ ! -f $BOOSTARCHIVE.tar.bz2 ]]; then
    echo "Downloading boost"
    sudo -E su -p kportal -c 'wget -nv http://heanet.dl.sourceforge.net/project/boost/boost/$BOOSTVERSION/$BOOSTARCHIVE.tar.bz2'
fi
if [[ ! -d "$BOOSTARCHIVE" ]]; then
    sudo -E su -p kportal -c 'bzip2 -dc "$BOOSTARCHIVE.tar.bz2" | tar xf -'
fi
sudo -E chmod -R 777 "$BOOSTARCHIVE"
cd "$BOOSTARCHIVE"
echo "Building boost library"
sudo touch $INSTALL_DIR/boostinstall.log
sudo chmod 666 $INSTALL_DIR/boostinstall.log
sudo ./bootstrap.sh --prefix=$KP_HOME/usr > $INSTALL_DIR/boostinstall.log
echo "Installing boost library. Logs are in $INSTALL_DIR/boostinstall.log."
sudo ./b2 install >> $INSTALL_DIR/boostinstall.log
echo "Boost installed into $KP_HOME/usr?"
ls -l $KP_HOME/usr/
echo "Building and installing kp_server"
cd $KP_HOME/src 
export CPLUS_INCLUDE_PATH=$KP_HOME/usr/include/
# Test environment in sudo su
sudo touch $INSTALL_DIR/make.log
sudo chmod 666 $INSTALL_DIR/make.log
sudo -E su -p kportal -c 'env | grep -i "include"'
sudo -E su -p kportal -c 'make' &> $INSTALL_DIR/make.log
sudo make install
echo "Check kp_server installation"
which kp_server
ls -l $(which kp_server)

cd $ORG_DIR