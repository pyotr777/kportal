CC=g++
RM=rm -r -f
CP=cp
INCDIR=-I.
OBJDIR=./obj
CFLAGS= $(INCDIR) -c -Wall -fPIC
LDFLAGS=-Wall
LDLIBRARIES=-lpthread

CPPFILES=message.cpp header.cpp system_command_utils.cpp \
         message_socket.cpp slavedaemon.cpp
OBJS=$(CPPFILES:%.cpp=$(OBJDIR)/%.o)
TARGET=slavedaemon
all : $(TARGET)
$(OBJDIR)/%.o : %.cpp
	$(CC) $(CFLAGS) $^ -o $@
$(TARGET) : $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBRARIES)
clean:
	$(RM) $(OBJDIR)/*.o $(TARGET)
install:
	$(CP) startslavedaemon.sh /bin
	$(CP) $(TARGET) /bin


