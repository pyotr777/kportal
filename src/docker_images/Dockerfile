FROM ubuntu:latest
MAINTAINER KportalTeam

RUN apt-get update && apt-get install -y wget g++ make
ADD slavedaemon.tar /
WORKDIR /slavedaemon
RUN ls -l
RUN make && make install
