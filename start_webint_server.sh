#!/bin/bash
# Start webint for installing K-portal.

cd $HOME/kportal/webint
mkdir logs 2> /dev/null || true
./webint.py > logs/webint.log 2>&1 &
echo "Web interface server started on port 8080."
exit
