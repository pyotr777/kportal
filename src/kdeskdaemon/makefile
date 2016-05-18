CC=g++
RM=rm -r -f
CP=cp
INCDIR=-I.
MESSINCDIR=-I../kpforwarder -I../slavedaemon/
OBJDIR=./obj
CFLAGS= $(INCDIR) $(MESSINCDIR) -c -Wall -fPIC
LDFLAGS=-Wall
LDLIBRARIES=-lpthread

CPPFILES=message.cpp header.cpp system_command_utils.cpp \
         message_socket.cpp kdeskdaemon.cpp
OBJS=$(CPPFILES:%.cpp=$(OBJDIR)/%.o)
TARGET=kdeskdaemon
all : $(TARGET)
$(OBJDIR)/%.o : ../kpforwarder/%.cpp
	$(CC) $(CFLAGS) $^ -o $@
$(OBJDIR)/%.o : ../slavedaemon/%.cpp
	$(CC) $(CFLAGS) $^ -o $@
$(OBJDIR)/%.o : %.cpp
	$(CC) $(CFLAGS) $^ -o $@
$(TARGET) : $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBRARIES)
	mkdir -p ../release/
	$(CP) $(TARGET) ../release/
clean:
	$(RM) $(OBJDIR)/*.o
install:
	mkdir -p ../release/
	$(CP) $(TARGET) ../release/
