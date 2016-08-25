#!/bin/bash
# Start webint for installing K-portal.

cd $HOME/kportal/webint
mkdir logs 2> /dev/null || true
./webint.py > logs/webint.log 2>&1 &

echo "Detecting IP address."
EXT_IP=$(curl -m 5 ifconfig.me 2>/dev/null || curl -m 5 icanhazip.com 2>/dev/null)
echo "IP detected: $EXT_IP"

echo "Web interface server started on http://$EXT_IP:8080."
exit
