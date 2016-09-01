#!/bin/bash
# Start Docker, Apache and kp_server

start_server.sh
start_apache.sh 9005
kp_server.sh 9004 -tls