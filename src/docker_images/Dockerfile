# Dockerfile use to create sample image or custom image.

FROM ubuntu:14.04
MAINTAINER KportalTeam

RUN apt-get update && apt-get install -y wget g++ make
ADD ./slavedaemon /slavedaemon
WORKDIR /slavedaemon
RUN make && make install
WORKDIR /
