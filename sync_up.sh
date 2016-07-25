OPT="-anv"
server="kpinstall"
if [[ $1 != "" ]]; then 
	if [[ $1 == "-do" ]]; then
		OPT="-av"
	else
		echo "Connecting to $1"
		server="$1"
	fi
fi
if [[ $2 == "-do" ]]; then
    OPT="-av"
fi
if [[ "$OPT" == *n* ]]; then
    echo "Dry run with options \"$OPT\". To actually transfer files execute with \"-do\" option: > $0 [server] -do"
fi
rsync $OPT --exclude-from 'rsyncexclude.txt' ./ $server:kportal/
