# Dockerfile use to create sample image or custom image.

FROM ubuntu:latest
MAINTAINER KportalTeam

RUN apt-get update && apt-get install -y wget g++ make
#RUN wget http://10.0.0.1:9005/slavedaemon.tar
RUN wget  http://bounceme.anlab.info:9005/slavedaemon.tar && tar -xvf  slavedaemon.tar && cd "/slavedaemon/" && make && make install
