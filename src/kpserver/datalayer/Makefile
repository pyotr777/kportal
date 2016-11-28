CC      	= g++
CFLAGS  	= -I. -c -Wall -fPIC
LDFLAGS 	= -Wall -shared -lpthread -o

SOURCES		= 	DataManager.cpp \
			user.cpp \
			service.cpp \
			parameter.cpp \
			user_service.cpp \
			image_docker.cpp
			
OBJECTS 	= $(SOURCES:.cpp=.o)
EXECUTABLE 	= datalayer.so

all: $(OBJECTS) sqlite3.o
	mv $(OBJECTS) ../bin
	mv sqlite3.o	../bin
	
#$(EXECUTABLE): $(OBJECTS) sqlite3.o
#	$(CC) $(OBJECTS) sqlite3.o $(LDFLAGS) $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

sqlite3.o:
	gcc $(CFLAGS) -DSQLITE_DEFAULT_FOREIGN_KEYS=1 sqlite3.c

clean:
	rm -f *.o
	rm -f $(EXECUTABLE)
	rm -f test

for_kp_tool: $(OBJECTS) sqlite3.o
	mv $(OBJECTS) ../../kptool/bin
	mv sqlite3.o	../../kptool/bin

test: $(OBJECTS) sqlite3.o test.o
	$(CC) $(OBJECTS) sqlite3.o test.o -Wall -lpthread -ldl -o $@
test.o: test.cpp
	$(CC) $(CFLAGS) test.cpp
