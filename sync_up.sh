#!/bin/bash
# Syncronise files with ~/kportal directory on the host.

read -rd '' USAGE <<EOF
	Script for copying updated K-portal installation files to remote machine.
	Usage:
	$(basename $0) [-i ssh key] [-e exclude pattern] [user@]host [-do]
	Whithout "-do" option a dry run will be perfomed. Use it to see the list of updated files.
EOF

if [[ "$#"<1 ]]; then
	echo "$USAGE"
	exit 1
fi

OPT="-anv"
DIR="kportal"
KEY=""
EXCLUDE=""


while test $# -gt 0; do
	case "$1" in
		-i)
			KEY="-e 'ssh -i $2'";shift;
			;;
		-e)
			EXCLUDE="$EXCLUDE --exclude '$2'";shift;
			;;
		-do)
			OPT="-av"
			echo "Live run"
			;;		
		--) 
			shift
			break;;
		-*)	
			echo "Invalid option: $1"
			echo "$USAGE"
			exit 1;;
		*)
			server="$1"
			;;
	esac
	shift
done


if [[ "$OPT" == *n* ]]; then
	echo "Dry run with options \"$OPT\". To actually transfer files run with \"-do\" option."
fi

eval rsync $OPT $KEY $EXCLUDE --exclude-from 'rsyncexclude.txt' ./ $server:$DIR/
