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
	$(info Installing kp_server to $(BIN_DIR).)
	mkdir -p $(PREFIX)
	mkdir -p $(PREFIX)/log
	mkdir -p $(PREFIX)/kdeskdaemon
	$(CP) -rf AppMarket/ssl/ $(PREFIX)
	$(CP) -rf $(APPMARKET_DIR) $(DOCUMENT_ROOT)
	$(CP) $(RELEASE_DIR)/kdeskdaemon $(PREFIX)/kdeskdaemon/
	chmod 666  $(PREFIX)/kdeskdaemon
	$(CP) $(RELEASE_DIR)/start_server.sh $(BIN_DIR)/
	$(CP) $(RELEASE_DIR)/start_docker.sh $(BIN_DIR)/
	$(CP) $(RELEASE_DIR)/kp_server.sh $(BIN_DIR)/
	$(CP) $(RELEASE_DIR)/kp_server $(BIN_DIR)/
	$(CP) $(RELEASE_DIR)/kp_tool $(BIN_DIR)/
	$(CP) $(RELEASE_DIR)/$(DEFAULT_JOBSH_FILE) $(PREFIX)
	$(CP) $(RELEASE_DIR)/$(KP_CONFIG_FILE) $(PREFIX)
	chmod 777 -R $(PREFIX)

uninstall:
	rm -rf $(PREFIX)
	rm -f $(BIN_DIR)/kp_server
	rm -f $(BIN_DIR)/kdeskdaemon
clean:
	$(CD) $(INCLUDE)/libtar-1.2.11; make clean
	$(CD) $(INCLUDE)/websocketpp-0.2.3-dev; make clean
	$(CD) $(KPSERVER_DIR); make clean
	$(CD) $(KDESKDAEMON_DIR); make clean
	$(CD) $(KPTOOL_DIR); make clean
