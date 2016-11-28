BOOST_LIBS=boost_system boost_thread boost_regex boost_random

include ../common.mk

all: 	CheckDir Lib Next
CheckDir:
	mkdir -p ../lib
Lib:
	cd ../include/websocketpp; make
	cd ../include/libjson; make
#CFLAGS = -I. -I./Objects -I./Utils -I./Exceptions

LDFLAGS := $(LDFLAGS) -lcrypto -lssl -lpthread -lrt
Next: k_client
k_client: main.cpp kp_client.a
	#$(CXX) $(CFLAGS) $^ -g -o $@ $(LDFLAGS) kp_client.a
	#$(CXX) main.cpp -g $(CFLAGS) -o $@ kp_client.a $(LDFLAGS)
	g++ main.cpp -g -Wall -O2 -o k_client  -I /usr/local  -L /usr/local/lib kp_client.a ../lib/libwebsocketpp.a ../lib/libjson.a -lboost_system -lboost_thread -lboost_regex -lboost_random -lcrypto -lssl -lpthread -lrt -lcurl

kp_client.a:  KP_Manager.o KP_Service.o KP_Job.o ResUtils.o KP_ParametersList.o KP_Parameter.o StringUtils.o FileUtils.o StringUtils.o
	ar crf $@ KP_Manager.o KP_Service.o KP_Job.o ResUtils.o KP_ParametersList.o KP_Parameter.o StringUtils.o FileUtils.o StringUtils.o

%.o: %.cpp
	$(CXX) -g -c $(CFLAGS) -o $@ $^

# cleanup by removing generated files
#
.PHONY:		clean
clean:
		rm -f *.o k_client kp_client.a ../lib/*.a
		cd ../include/websocketpp; make clean
		cd ../include/libjson; make clean

install:
	    cp k_client /usr/bin/

uninstall:
	    rm -rf /usr/bin/k_client
