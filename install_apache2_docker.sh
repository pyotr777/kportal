#!/bin/bash
# Build Docker image with Apache2 with ssl support

echo "Building apachessl Docker image"
docker build --rm -t apachessl .
docker images