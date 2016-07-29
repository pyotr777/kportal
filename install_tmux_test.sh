#!/bin/bash -e
# Copy installation files from current directory to remote machine
# and start remote installation.
# Require 1 parameter: remote [user@]hostname.

if [[ $# < 1 ]]; then
	echo "Usage:"
	echo "$(basename $0) [user@]hostname"
	echo "Requires working ssh connection to hostname."
	exit 0
fi

REMOTE=$1

ssh $REMOTE hostname 2>/dev/null || (echo "Cannot connect with 'SSH $REMOTE'." && exit 1)

SSH_OPTIONS="-o ServerAliveInterval=15 -t"
# Copy files to remote
echo "Synchronising installation files with remote location."
./sync_up.sh $REMOTE -do

echo "Starting remote installation web-server."

ssh $REMOTE $SSH_OPTIONS kportal/start_tmux_test.sh