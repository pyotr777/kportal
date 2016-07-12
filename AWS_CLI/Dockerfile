FROM ubuntu:14.04
MAINTAINER Peter Bryzgalov

# Amazon AWS CLI installer in container

RUN apt-get update && apt-get install -y python-setuptools python-dev build-essential python-pip groff ssh jq
RUN pip install awscli

# Shortcut commands

ADD shortcut_commands/* /usr/local/bin/

RUN chmod +x /usr/local/bin/aws*
CMD ["/bin/bash"]
