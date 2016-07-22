#!/bin/bash
#
# Start Docker on port 9555 of localhost on bridge0 if,
# create bridge0 if not exists.

# Detect Travis

if [[ "$HOME" = *travis* ]]; then
	export TRAVIS=yes;
fi

if [[ -z $TRAVIS ]]; then
	# Check Docker status
	# If Docker runs on 9555
	docker -H 127.0.0.1:9555 images &>/dev/null
	if [[ $? -eq 0 ]]; then
		echo "Docker is running on 9555 port"
		exit 0
	fi
	# Stop docker
	sudo docker images &>/dev/null
	if [[ $? -eq 0 ]]; then
		echo "Docker is running as a service. Stopping it."
		sudo service docker stop
		sleep 5
	fi	

	# Start bridge & start docker 
	if [ ! "$(brctl show | grep bridge0)" ]; then
		sudo brctl addbr bridge0
		sudo ip addr add 10.0.0.1/8 dev bridge0
		sudo ip link set dev bridge0 up
		ip addr show bridge0
	fi
	sudo -E su kportal -c 'docker -H 127.0.0.1:9555 images &>/dev/null'
	if [[ $? -ne 0 ]]; then
		echo "Starting Docker on bridge0, port 9555."
		sudo start_docker.sh 
		sleep 10
	fi
	# If Docker couldnt start on 9555, run socat 
	sudo -E su kportal -c 'docker -H 127.0.0.1:9555 images &>/dev/null'
	if [[ $? -ne 0 ]]; then
		echo "Docker not accessible on port 9555. "
		exit 1
		sudo socat TCP4-LISTEN:9555,fork UNIX-CLIENT:/var/run/docker.sock &
	else
		echo "Docker is running on port 9555."
	fi
else 
	socat &> /dev/null
	if [[ $? -ne 0 ]]; then
		echo "Installing socat"
		sudo apt-get install -y socat 
	fi
	sudo ps ax | grep "socat" | grep 9555 &> /dev/null
	if [[ $? -ne 0 ]]; then
		sudo socat TCP4-LISTEN:9555,fork UNIX-CLIENT:/var/run/docker.sock &
		export PID=$!
		echo "Socat PID is $PID"
	else
		sudo ps ax | grep "socat" | grep 9555
	fi
fi
