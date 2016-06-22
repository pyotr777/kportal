#!/bin/bash
# Build Docker image with Apache2 with ssl support

echo "Building apachessl Docker image"
sudo docker build --rm -t apachessl .
sudo docker images