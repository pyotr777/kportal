#!/bin/bash
# Start apache
sudo service httpd start

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
    sudo docker daemon -D -H 127.0.0.1:9555 -b=bridge0 --storage-driver=devicemapper --storage-opt dm.datadev=/dev/vg-docker/data --storage-opt dm.metadatadev=/dev/vg-docker/metadata &
fi
