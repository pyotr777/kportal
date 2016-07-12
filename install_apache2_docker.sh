#!/bin/bash
# Build Docker image with Apache2 with ssl support
pwd
echo "Building apache Docker image"
sudo docker build --rm -t apache . > /dev/null
sudo docker images