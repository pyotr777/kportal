#!/bin/bash
docker daemon -D -H 127.0.0.1:9555 -b=bridge0 &> /home/kportal/log/docker.log &
