PREFIX		= /etc/kportal
CP			= cp
CD			= cd
KPSERVER_DIR = ./kpserver
KDESKDAEMON_DIR = ./kdeskdaemon
RELEASE_DIR = ./release
KP_CONFIG_FILE = kportal_conf.xml
DEFAULT_JOBSH_FILE = default_job_script.sh
APPMARKET_DIR = ./AppMarket
DOCUMENT_ROOT = $(PREFIX)/www
KPTOOL_DIR = ./kptool
INCLUDE = ./include
BIN_DIR = /usr/local/bin

all: 
	$(CD) $(INCLUDE)/libtar-1.2.11; make; cp lib/libtar.a  ../../lib
	$(CD) $(INCLUDE)/websocketpp-0.2.3-dev; make
	$(CD) $(KPTOOL_DIR); make
	$(CD) $(KDESKDAEMON_DIR); make
	$(CD) $(KPSERVER_DIR); make
	
install:
	mkdir -p $(PREFIX)
	mkdir -p $(PREFIX)/log
	$(CP) -rf ssl/ $(PREFIX)
	$(CP) -rf $(APPMARKET_DIR) $(DOCUMENT_ROOT)
	$(CP) $(RELEASE_DIR)/kdeskdaemon $(BIN_DIR)/
	chmod 666  $(BIN_DIR)/kdeskdaemon
	$(CP) $(RELEASE_DIR)/start_server.sh $(BIN_DIR)/
	$(CP) $(RELEASE_DIR)/kp_server.sh $(BIN_DIR)/
	$(CP) $(RELEASE_DIR)/kp_server $(BIN_DIR)/
	$(CP) $(RELEASE_DIR)/kp_tool $(BIN_DIR)/
	$(CP) $(RELEASE_DIR)/$(DEFAULT_JOBSH_FILE) $(PREFIX)
	$(CP) $(RELEASE_DIR)/$(KP_CONFIG_FILE) $(PREFIX)
	$(info Installed kp_server to $(BIN_DIR).)
	chmod 777 -R $(PREFIX)	
uninstall:
	rm -rf $(PREFIX)
clean:
	$(CD) $(INCLUDE)/libtar-1.2.11; make clean
	$(CD) $(INCLUDE)/websocketpp-0.2.3-dev; make clean
	$(CD) $(KPSERVER_DIR); make clean
	$(CD) $(KDESKDAEMON_DIR); make clean
	$(CD) $(KPTOOL_DIR); make clean
