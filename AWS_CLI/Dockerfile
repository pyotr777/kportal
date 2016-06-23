FROM ubuntu:14.04
MAINTAINER Peter Bryzgalov

# Amazon AWS CLI installer in container

RUN apt-get update && apt-get install -y python-setuptools python-dev build-essential python-pip groff ssh jq
RUN pip install awscli

# Shortcut commands

RUN printf "#!/bin/bash\naws ec2 describe-instances --query 'Reservations[*].{ID:Instances[*].InstanceId,TAG:Instances[*].Tags,STATE:Instances[*].State.Name}' | jq .\n" > /usr/local/bin/awsls
RUN printf "#!/bin/bash\naws ec2 run-instances --cli-input-json file://\$1\n" > /usr/local/bin/awsrun
RUN printf "#!/bin/bash\naws ec2 describe-instances --instance-ids \$1 --query 'Reservations[0].Instances[0].PublicIpAddress'\n" > /usr/local/bin/awsip
RUN printf "#!/bin/bash\naws ec2 stop-instances --instance-ids \$1" > /usr/local/bin/awsstop
RUN printf "#!/bin/bash\naws ec2 terminate-instances --instance-ids \$1\n" > /usr/local/bin/awsterminate
RUN printf "#!/bin/bash\naws ec2 create-tags --resources $1 --tags Value=$2,Key=Name\n" > /usr/local/bin/awstag
RUN printf "#!/bin/bash\nls -l /usr/local/bin/aws*\n" > /usr/local/bin/awshelp

RUN chmod +x /usr/local/bin/aws*
CMD ["/bin/bash"]