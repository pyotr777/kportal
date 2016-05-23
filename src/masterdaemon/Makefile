CC=g++
RM=rm -r -f
CP=cp
INCDIR=-I.
OBJDIR=./obj
CFLAGS= $(INCDIR) -c -Wall -fPIC
LDFLAGS=-Wall -lpthread

CPPFILES=server.cpp 
OBJS=$(CPPFILES:%.cpp=$(OBJDIR)/%.o)
TARGET=masterdaemon
all : $(TARGET)
$(OBJDIR)/%.o : %.cpp
	$(CC) $(CFLAGS) $^ -o $@
$(TARGET) : $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@
clean:
	$(RM) $(OBJDIR)/*.o $(TARGET)
install:
	$(CP) start_server.sh /
	$(CP) $(TARGET) /bin
