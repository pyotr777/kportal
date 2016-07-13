#!/bin/bash

LOCKFILE="/run/apache2/apache2.pid"
if [[ -f "$LOCKFILE" ]]; then
	echo "Removing lockfile $LOCKFILE"
	rm -f $LOCKFILE
fi
/usr/sbin/apache2ctl -D FOREGROUND