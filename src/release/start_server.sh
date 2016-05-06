# Start apache
~/usr/local/apache/bin/apachectl start

# Stop docker
sudo service docker stop;

# Start bridge & start docker 
sudo brctl addbr bridge0
sudo ip addr add 10.0.0.1/8 dev bridge0
sudo ip link set dev bridge0 up
ip addr show bridge0
sudo docker -d -H 127.0.0.1:9555 -b=bridge0 &

