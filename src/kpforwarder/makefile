CC      	= g++
CFLAGS  	= -I. -I../slavedaemon -I../kpforwarder -c -Wall -fPIC -std=c++11
LDFLAGS 	= -Wall -std=c++11 -lpthread -o

SOURCES		= 	header.cpp \
			message.cpp \
			message_socket.cpp \
			tunnel.cpp \
			system_command_utils.cpp \
			kpforwarder.cpp \

OBJDIR		= ./obj

#OBJECTS 	= $(SOURCES:.cpp=.o)
OBJECTS		= $(SOURCES:%.cpp=$(OBJDIR)/%.o)
EXECUTABLE 	= kpforwarder
all: $(EXECUTABLE)
#.cpp.o:
#	$(CC) $(CFLAGS) $< -o $@
$(OBJDIR)/%.o : %.cpp
	$(CC) $(CFLAGS) $< -o $@
$(OBJDIR)/%.o : ../slavedaemon/%.cpp
	$(CC) $(CFLAGS) $^ -o $@
$(EXECUTABLE): $(OBJECTS)	
	$(CC) $(OBJECTS) $(LDFLAGS) $@

clean:
	rm -f $(OBJDIR)/*.o $(EXECUTABLE)
