#!/bin/bash

sudo modprobe aufs 
if [ $? -ne 0 ]; then
    echo "Installing AUFS"
    sudo apt-get install -y linux-image-extra-$(uname -r)
fi
