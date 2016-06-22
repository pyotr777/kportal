#!/bin/bash
#
# Start Docker on port 9555 of localhost on bridge0 if,
# create bridge0 if not exists.

# Stop docker
sudo service docker stop

# Start bridge & start docker 
if [ ! "$(brctl show | grep bridge0)" ]; then
    sudo brctl addbr bridge0
    sudo ip addr add 10.0.0.1/8 dev bridge0
    sudo ip link set dev bridge0 up
    ip addr show bridge0
fi
docker -H 127.0.0.1:9555 images &>/dev/null
if [[ "$?"=="1" ]]; then
    sudo docker daemon -D -H 127.0.0.1:9555 -b=bridge0 &
fi
# If Docker couldnt start on 9555, run socat 
docker -H 127.0.0.1:9555 images &>/dev/null
if [[ "$?"=="1" ]]; then
	sudo socat TCP4-LISTEN:9555,fork UNIX-CLIENT:/var/run/docker.sock &
fi