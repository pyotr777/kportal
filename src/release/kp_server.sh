#!/bin/sh
HOME_DIR=$(sudo -u kportal echo $HOME)
LOGDIR="$HOME_DIR/log"
echo "Starting kp_server. HOME_DIR=$HOME_DIR, LOGDIR=$LOGDIR."
if [ -z  "$LD_LIBRARY_PATH" ]; then 
	LD_LIBRARY_PATH=$HOME_DIR/usr/local/lib
else
	LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HOME_DIR/usr/local/lib
fi
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HOME_DIR/usr/lib

export LD_LIBRARY_PATH;

echo $LD_LIBRARY_PATH
if [ ! -d "$LOGDIR" ]; then
	mkdir -p "$LOGDIR"
fi
/bin/kp_server $@ 1>$LOGDIR/stdout.log 2>$LOGDIR/stderr.log
