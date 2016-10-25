FROM ubuntu:16.04
MAINTAINER Peter Bryzgalov <peterbryz@yahoo.com> @ CHITEC, Stair lab.

# Install Flask server and VTK

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends  apt-utils
RUN apt-get install -y --no-install-recommends \
    checkinstall bzip2 ca-certificates sudo locales \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

#RUN echo "deb http://www.deb-multimedia.org jessie main non-free" >> /etc/apt/sources.list
#RUN apt-get update
#RUN apt-get install -y --force-yes deb-multimedia-keyring
RUN apt-get update

RUN apt-get install -y build-essential ffmpeg && apt-get -y autoremove
RUN apt-get install -y libtiff5-dev libjpeg8-dev && apt-get -y autoremove
RUN apt-get install -y libmp3lame-dev libvorbis-dev libtheora-dev \
    libspeex-dev yasm pkg-config libopenjpeg-dev libx264-dev && apt-get -y autoremove
RUN apt-get install -y libvtk5-dev libvtk5.10 && apt-get -y autoremove
RUN apt-get install -y python-vtk python-pyvtk && apt-get -y autoremove
RUN apt-get install -y python-setuptools python-pip && apt-get -y autoremove
RUN apt-get install -y python-matplotlib python-numpy python-scipy python-pandas \
    python-tk && apt-get -y autoremove

RUN pip install -U pip
RUN pip install -U matplotlib
RUN pip install -U Pillow
RUN pip install -U Flask

RUN /usr/bin/python --version
RUN pip freeze

WORKDIR /root/server
ENV FLASK_APP=server.py

