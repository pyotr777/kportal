#!/bin/bash
HOME_DIR=$HOME
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
nohup kp_server $@ 1>$LOGDIR/stdout.log 2>$LOGDIR/stderr.log & 
PID="$!"
echo "Is kp_server running? PID=$PID"
if [[ "$PID" ]]; then
	ps aux | grep "$PID"
fi
echo "Check kp_server logs"
echo "STDOUT"
cat $LOGDIR/stdout.log
echo "STDERR"
cat $LOGDIR/stderr.log