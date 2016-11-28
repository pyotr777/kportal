#BOOST_LIBS=boost_system boost_date_time boost_program_options boost_thread boost_regex

include ../../common.mk

CC      	= g++
CFLAGS  	= -I $(BOOST_INCLUDE_PATH) -O2 -fpic -I. -c -Wall -g
#LDFLAGS 	= -Wall -shared -lcrypto -lssl

LDFLAGS     := $(LDFLAGS) -Wall -shared -lcrypto -lssl -lpthread -lrt -lboost_regex -ltar


KP_LFLAGS := ../../lib/libwebsocketpp.a ../../lib/libtar.a
KP_LFLAGS := $(KP_LFLAGS) ../../lib/libjson.a

SOURCES		= ClientSession.cpp \
		GmailUtils.cpp \
		ReqUtils.cpp    \
		JsonUtils.cpp	\
		FileUtils.cpp   \
		StringUtils.cpp	\
		NetworkUtil.cpp    \
		LoginManager.cpp    \
		TarUtil.cpp	\
		FileNetworkUtils.cpp \
		ssh_utils.cpp \
		tunnel_manager.cpp \
		CSmtp.cpp \
		md5.cpp \
		base64.cpp \
		
		#forwarder_apis.cpp  \
		#UploadFileManager.cpp	\
		#k_server_handler.cpp \

			
OBJECTS 	= $(SOURCES:.cpp=.o)

SO 	= auth.so

all: $(OBJECTS)
	mv $(OBJECTS) ../bin
	
#$(SO): $(OBJECTS)
#		$(CC) $(LDFLAGS) $(KP_LFLAGS) $(OBJECTS) -lpthread -fPIC -o $@
#		mv $(OBJECTS) ../bin

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	rm -rf $(SO) *.o
