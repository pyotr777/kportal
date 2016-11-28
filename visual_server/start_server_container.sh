#!/bin/bash

# Start container with Flask visualisation server, mounting current dirctory.

docker run -ti --rm --name flask -v $(pwd):/root/server -p 8080:5000 pyotr777/vis-serv
