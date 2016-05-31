#!/bin/sh
if [ -z  "$LD_LIBRARY_PATH" ]; then 
	LD_LIBRARY_PATH=/home/anlab/usr/local/lib
else
	LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/anlab/usr/local/lib
fi
#LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/kportal/usr/lib

export LD_LIBRARY_PATH;
#export $LD_LIBRARY_PATH
echo $LD_LIBRARY_PATH
/bin/kp_server $@ 1>/etc/kportal/log/stdout.log 2>/etc/kportal/log/stderr.log
