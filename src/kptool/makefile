include ../common.mk
BIN		= ./bin	
CC      	= g++
CFLAGS  	= -I. -I../kpserver/datalayer -c -Wall
LDFLAGS 	= -Wall -lpthread -ldl

SOURCES 	= main.cpp
			
OBJECTS 	= $(SOURCES:.cpp=.o)

#SRC		= $(wildcard ./bin/*.o)
SRC		= ./bin/DataManager.o ./bin/parameter.o ./bin/user.o ./bin/service.o ./bin/sqlite3.o ./bin/user_service.o ./bin/image_docker.o

EXECUTABLE 	= kp_tool

all: clean CheckFolder datalayer next

CheckFolder:
	mkdir -p $(BIN)

datalayer:
	cd ../kpserver/datalayer; make for_kp_tool

next: $(SOURCES) $(EXECUTABLE) $(SRC)

$(EXECUTABLE): $(OBJECTS) $(SRC)
	$(CC)  $(OBJECTS) $(SRC) -o $@ $(LDFLAGS)
	mkdir -p ../release
	cp $(EXECUTABLE) ../release

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	 rm -rf $(OBJECTS)
	 rm -rf $(SRC)
	 rm -rf $(EXECUTABLE)
	 rm -f	../release/$(EXECUTABLE)
	 cd ../kpserver/datalayer; make clean
