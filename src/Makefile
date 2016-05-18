PREFIX		= /etc/kportal
CP			= cp
CD			= cd
KPSERVER_DIR = ./kpserver
KDESKDAEMON_DIR = ./kdeskdaemon
RELEASE_DIR = ./release
KP_CONFIG_FILE = kportal_conf.xml
APPMARKET_DIR = ./AppMarket
DOCUMENT_ROOT = $(PREFIX)/www
KPTOOL_DIR = ./kptool
INCLUDE = ./include


all: 
	$(CD) $(INCLUDE)/libtar-1.2.11; make; cp lib/libtar.a  ../../lib
	$(CD) $(INCLUDE)/websocketpp-0.2.3-dev; make
	$(CD) $(KPTOOL_DIR); make
	$(CD) $(KDESKDAEMON_DIR); make
	$(CD) $(KPSERVER_DIR); make
	
install:
	mkdir -p $(PREFIX)
	mkdir -p $(PREFIX)/log
	chmod 777 -R $(PREFIX)
	$(CP) -rf $(INCLUDE)/ssl/ $(PREFIX)
	$(CP) -rf $(APPMARKET_DIR) $(DOCUMENT_ROOT)
	$(CP) $(RELEASE_DIR)/kdeskdaemon /bin/
	chmod 666  /bin/kdeskdaemon
	$(CP) $(RELEASE_DIR)/start_server.sh /bin/
	$(CP) $(RELEASE_DIR)/kp_server.sh /bin/
	$(CP) $(RELEASE_DIR)/kp_server /bin/
	$(CP) $(RELEASE_DIR)/kp_tool /bin/
	$(CP) $(RELEASE_DIR)/$(KP_CONFIG_FILE) $(PREFIX)
	
uninstall:
	rm -rf $(PREFIX)
clean:
	$(CD) $(INCLUDE)/libtar-1.2.11; make clean
	$(CD) $(INCLUDE)/websocketpp-0.2.3-dev; make clean
	$(CD) $(KPSERVER_DIR); make clean
	$(CD) $(KDESKDAEMON_DIR); make clean
	$(CD) $(KPTOOL_DIR); make clean
