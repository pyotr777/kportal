#!/bin/bash
# Start webint for installing K-portal.

cd $HOME/kportal/webint
mkdir logs
./webint.py > logs/webint.log 2>&1 &
exit
