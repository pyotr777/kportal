#!/bin/bash
# Start Firefox on server to use as webint user interface.

echo "Starting browser to continue installation."
echo "http://$1"
echo ""

sleep 5
#cd $HOME/kportal/webint

(echo "Trying Firefox "; firefox http://$1 2>/dev/null) || (echo "Trying open command "; open http://$1 2>/dev/null) \
		|| (echo "Trying xdg-open command "; xdg-open http://$1 2>/dev/null) || echo -en "To continue installation open this page in your browser:\n\033[38;5;27mhttp://$1:8080\033[m\n"



