#!/bin/bash
# Start Docker, Apache and kp_server

/usr/local/bin/start_server.sh
/usr/local/bin/start_apache.sh 9005
sudo -E su kportal /usr/local/bin/kp_server.sh 9004 -tls

