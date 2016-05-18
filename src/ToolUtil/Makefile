all: server client
server:
	g++ server.cpp -o server -lpthread
	mkdir -p ../release/base_image_setup/bin/
	cp server ../release/base_image_setup/bin/
client:
	g++ client.cpp -o client

clean:
	rm -f server
	rm -f client
