CC      	= g++
CFLAGS  	= -I. -c -Wall -fPIC 
LDFLAGS 	= -Wall -shared -lpthread -lgomp

SOURCES		= 	param_job.cpp \
			timer.cpp \
			job.cpp \
			job_manager.cpp 
			
OBJECTS 	= $(SOURCES:.cpp=.o)
EXECUTABLE 	= job_manager.so

all: $(OBJECTS)
	mv $(OBJECTS) ../bin
	
#$(EXECUTABLE): $(OBJECTS)
#	$(CC) $(OBJECTS) $(LDFLAGS) $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o
	rm -f $(EXECUTABLE)
