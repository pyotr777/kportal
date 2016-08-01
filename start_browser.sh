#!/bin/bash
# Start Firefox on server to use as webint user interface.

echo "Opening the following address in your Internet browser to continue installation:"
echo "http://$1"
echo ""

sleep 5
#cd $HOME/kportal/webint

(echo "Trying Firefox "; firefox http://$1 2>/dev/null) || (echo "Trying open command "; open http://$1 2>/dev/null) \
		|| (echo "Trying xdg-open command "; xdg-open http://$1 2>/dev/null) || echo "Open $1 address in your browser."





