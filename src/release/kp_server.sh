#!/bin/bash
HOME_DIR=$HOME
LOGDIR="$HOME_DIR/log"
PID=$(ps ax | grep "kp_server " | grep 9004 | grep -v "grep" | awk '{ print $1 }')
if [[ "$PID" ]]; then
	echo "Already running."
	ps ax | grep "$PID" | grep -v "grep"
	echo "Killing PID $PID"
	kill $PID
	sleep 5
fi

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
nohup /usr/local/bin/kp_server $@ 1>$LOGDIR/stdout.log 2>$LOGDIR/stderr.log & 
PID="$!"
echo "Is kp_server running? PID=$PID"
if [[ "$PID" ]]; then
	ps aux | grep "$PID" | grep -v "grep"
fi
echo "Check kp_server logs"
echo "STDOUT"
cat $LOGDIR/stdout.log
echo "STDERR"
cat $LOGDIR/stderr.log
ps ax | grep "kp_server"
echo "----------------"
