#!/bin/bash
# Start Firefox on server to use as webint user interface.

echo "Opening the following address in your Internet browser to continue installation:"
echo "http://$1"
echo ""

sleep 5
#cd $HOME/kportal/webint
echo "Try with Firefox"
firefox http://$1 || ( echo "Try open."; open http://$1) \
		|| (echo "Try xdg-open."; xdg-open http://$1)





