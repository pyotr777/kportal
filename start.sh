#!/bin/bash
# Start Firefox on server to use as webint user interface.

PORT=8080

echo "Opening the following address in your Internet browser to continue installation:"
echo "http://$1:$PORT"
echo ""
sleep 3
cd $HOME/kportal/webint
firefox http://localhost:$PORT



