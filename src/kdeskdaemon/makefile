CC=g++
RM=rm -r -f
CP=cp
INCDIR=-I.
MESSINCDIR=-I../kpforwarder -I../slavedaemon/
OBJDIR=./obj
CFLAGS= $(INCDIR) $(MESSINCDIR) -c -Wall -fPIC
LDFLAGS=-Wall
LDLIBRARIES=-lpthread
MKDIR_P= mkdir -p 

CPPFILES=message.cpp header.cpp system_command_utils.cpp \
         message_socket.cpp kdeskdaemon.cpp
OBJS=$(CPPFILES:%.cpp=$(OBJDIR)/%.o)
TARGET=kdeskdaemon
all : directories $(TARGET)
directories:
	$(MKDIR_P) $(OBJDIR)
$(OBJDIR)/%.o : ../kpforwarder/%.cpp
	$(CC) $(CFLAGS) $^ -o $@
$(OBJDIR)/%.o : ../slavedaemon/%.cpp
	$(CC) $(CFLAGS) $^ -o $@
$(OBJDIR)/%.o : %.cpp
	$(CC) $(CFLAGS) $^ -o $@
$(TARGET) : $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@  $(LDLIBRARIES)
	$(MKDIR_P) -p ../release/
	$(CP) $(TARGET) ../release/
clean:
	$(RM) $(OBJDIR)/*.o
install:
	$(MKDIR_P) ../release/
	$(CP) $(TARGET) ../release/
