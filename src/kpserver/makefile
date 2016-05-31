BOOST_LIBS=boost_system boost_date_time boost_program_options boost_thread boost_regex

include ../common.mk
OBJDIR 			= ../../../../release/

#LDFLAGS := $(LDFLAGS) -lboost_regex -lldap -lcrypto -lssl
LDFLAGS := $(LDFLAGS) -lboost_regex -lcrypto -lssl -ltar  -lgomp

KP_LFLAGS := ../lib/libwebsocketpp.a

KP_LFLAGS := $(KP_LFLAGS) ../lib/libjson.a

#SLAVECONTROL 	= $(OBJDIR)/SlaveControl
BIN				= ./bin
CC      	= g++
CFLAGS  	= -I $(BOOST_INCLUDE_PATH) -I. -Wall -g  -I./authenticate -I./datalayer
OFLAGS  	= -I $(BOOST_INCLUDE_PATH) -I. -c -Wall -g -I./authenticate -I./datalayer
REFERFLAGS  = -I../kpforwarder -I../slavedaemon -c -Wall -g 

SOURCES		= main.cpp functions.cpp
			
SRC			= $(wildcard ./bin/*.o)

OBJECTS 	= $(SOURCES:.cpp=.o)


REFER_SOURCES = header.cpp message.cpp message_socket.cpp system_command_utils.cpp

REFER_OBJECTS = $(REFER_SOURCES:%.cpp=$(BIN)/%.o)


EXECUTABLE 	= kp_server

#all: clean CheckFolder Auth DataLayer JobManager Next
all: CheckFolder Auth DataLayer JobManager Next
CheckFolder:
	mkdir -p $(BIN)
	mkdir -p ../lib
Lib:
	cd ../include/websocketpp; make
	cd ../include/libjson; make
Auth:
	cd ./authenticate; make
DataLayer:
	cd ./datalayer; make
JobManager:
	cd ./job_manager; make

Next: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(REFER_OBJECTS) $(OBJECTS)
	$(CC) $(CFLAGS) $(SRC) $(OBJECTS) -L../lib/ -ltar -lpthread -ldl -o $@ $(KP_LFLAGS) $(LDFLAGS)
	mkdir -p ../release
	cp $(EXECUTABLE) ../release
.cpp.o:
	$(CC) $(LDFLAGS) $(OFLAGS) $< -o $@
$(BIN)/%.o : ../kpforwarder/%.cpp
	$(CC) $(REFERFLAGS) $^ -o $@
$(BIN)/%.o : ../slavedaemon/%.cpp
	$(CC) $(REFERFLAGS) $^ -o $@
clean:
	rm -f $(OBJECTS)
	rm -f $(EXECUTABLE)
	rm -f ./bin/*.o
	rm -f ../release/$(EXECUTABLE)
	cd ./authenticate; make clean
	cd ./datalayer; make clean
	cd ./job_manager; make clean
