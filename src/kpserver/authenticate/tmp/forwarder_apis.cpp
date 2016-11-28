#include "forwarder_apis.hpp"

// Global static pointer used to ensure a single instance of the class.
ForwarderApis* ForwarderApis::m_instance = NULL;


ForwarderApis::ForwarderApis()
{
    //std::stringstream ss;
    //ss << processName << " " << listenPort << ";";
    int processId = fork();
    // Child process
    if (processId == 0)
    {
        // Start listen port
        StartListen();
    }
    // Parent process
    else if(processId)
    {
        // TODO: nothing
    }
    else
    {
       // fork failed
       printf("Forking failed!\n");
      // exit(1);
    }
}

ForwarderApis::~ForwarderApis()
{
    if(m_instance)
        delete m_instance;
}

ForwarderApis* ForwarderApis::GetInstance()
{
    if(m_instance == NULL)
        m_instance = new ForwarderApis();
    return m_instance;
}

bool ForwarderApis::IsStarted()
{
    return true;
}

//
// Start socket listen connection from special port
//
int ForwarderApis::StartListen()
{
    int socket_desc , client_sock , c , *new_sock;
    struct sockaddr_in server , client;

    // Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = m_listen_port;

    // Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    // Listen
    listen(socket_desc , 5);

    // Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while( true )
    {
        client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
        if (client_sock < 0)
        {
            perror("accept failed");
            continue;
        }
        puts("Connection accepted");

        pthread_t sniffer_thread;
        new_sock = (int*)malloc(1);
        *new_sock = client_sock;

        if( pthread_create( &sniffer_thread , NULL ,  NULL , (void*) new_sock) < 0)
        {
            perror("could not create thread");
            continue;
        }

        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( sniffer_thread , NULL);
        puts("Handler assigned");
    }
    return 0;
}

//
// This will handle connection for each client
// Format message: (Type message) - 1byte, (size of message) - sizeof(BYTE), (content) - ...
//
/*void * ForwarderApis::ConnectionHandler(void *socket_desc)
{
    /*
    // Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char buffer[BUFSIZ];

    // Receive a message from client
    while( (read_size = recv(sock , buffer , BUFSIZ , 0)) > 0 )
    {
//        write(sock , buffer , read_size);
        std::cout << "Received signal from client\n";
        Message mes;
//        unsigned long remain_message = 0;
        std::string message = "";
        mes.typeMes = (TypeMessage)buffer[0];

        if (read_size > (sizeof(unsigned long) + sizeof(BYTE))) {
            mes.sizeMes = *(unsigned long*)(buffer + sizeof(BYTE));
            message += std::string(buffer + sizeof(unsigned long) + sizeof(BYTE), read_size - sizeof(unsigned long) - sizeof(BYTE));
        }

        ResultCode res = Success;
        if (message.size() >= mes.sizeMes) {
            mes.content = message.substr(0, mes.sizeMes);
            res = ProcessMessage(mes, sock);
        } else {
            while ( (read_size = recv(sock , buffer , BUFSIZ , 0)) > 0 ) {
                message += std::string (buffer, read_size);
                if (message.size() >= mes.sizeMes) {
                    mes.content = message.substr(0, mes.sizeMes);
                    res = ProcessMessage(mes, sock);
                    break;
                }
            }
        }

        Message mesToSend;
        mesToSend.typeMes = Binary;
        mesToSend.sizeMes = sizeof(BYTE);
        BYTE temp = res;
        mesToSend.content = std::string((char*)&temp, sizeof(BYTE));
        SendMessage(sock, mesToSend);
//        BYTE temp = res;
//        write(sock , &temp , sizeof(BYTE));
//        close(sock);
    }

    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }

    //Free the socket pointer
    free(socket_desc);

}*/
