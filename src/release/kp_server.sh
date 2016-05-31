#!/bin/sh
if [ -z  "$LD_LIBRARY_PATH" ]; then 
	LD_LIBRARY_PATH=/home/kportal//usr/local/lib
else
	LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/kportal//usr/local/lib
fi
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/kportal/usr/lib

export LD_LIBRARY_PATH;
#export $LD_LIBRARY_PATH
echo $LD_LIBRARY_PATH
/bin/kp_server $@ 1>/home/kportal/log/stdout.log 2>/home/kportal/log/stderr.log
