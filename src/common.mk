#BOOST_PREFIX ?= /home/hpcutest1/kportal/usr/local
#BOOST_PREFIX ?= /home/kportal/usr
#BOOST_PREFIX ?= /home/anlab/usr/local
BOOST_PREFIX ?= /usr
BOOST_LIB_PATH		?= $(BOOST_PREFIX)/lib
BOOST_INCLUDE_PATH ?= $(BOOST_PREFIX)/include

#KPSERVER_PATH ?= ..

CPP11               ?= 

CFLAGS = -Wall -O2 $(CPP11) -I $(BOOST_INCLUDE_PATH)
LDFLAGS = -L$(BOOST_LIB_PATH)

CXX		?= c++
SHARED  ?= 0

ifeq ($(SHARED), 1)
	LDFLAGS := $(LDFLAGS) -lwebsocketpp	
	LDFLAGS := $(LDFLAGS) $(BOOST_LIBS:%=-l%)
else
#	LDFLAGS := $(LDFLAGS) ../lib/libwebsocketpp.a
#	LDFLAGS := $(LDFLAGS) ../../lib/libwebsocketpp.a
#	LDFLAGS := $(LDFLAGS) ../lib/libjson.a
#	LDFLAGS := $(LDFLAGS) ../../lib/libjson.a
#	LDFLAGS := $(LDFLAGS) $(KPSERVER_PATH)/libjson.a
#	LDFLAGS := $(LDFLAGS) $(KPSERVER_PATH)/libwebsocketpp.a
	LDFLAGS := $(LDFLAGS) -lcurl
	LDFLAGS := $(LDFLAGS) $(BOOST_LIBS:%=$(BOOST_LIB_PATH)/lib%.a)

endif
