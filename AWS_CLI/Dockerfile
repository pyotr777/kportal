FROM ubuntu:14.04
MAINTAINER Peter Bryzgalov

# Amazon AWS CLI installer in container

RUN apt-get update && apt-get install -y python-setuptools python-dev build-essential python-pip groff
RUN pip install awscli
RUN aws help
CMD ["/bin/bash"]