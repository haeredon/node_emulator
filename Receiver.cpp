#include "Receiver.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

Receiver::Receiver(ReceiverConfig config) : Node(std::forward<ReceiverConfig>(config)) { }


void Receiver::start() {  
    int socketDescriptor = getSocket();

    const struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(config.getListeningPort()),
        .sin_addr = {
            .s_addr = INADDR_ANY
        }
    };    

    int ret = bind(socketDescriptor, (struct sockaddr*) &addr, sizeof(addr));

    if(ret == -1) {
        printf("Failed to bind socket: %d \n", errno);
        throw;
    }

    ret = listen(socketDescriptor, 10 /* backlog */);

    if(ret == -1) {
        printf("Failed to listen on socket: %d \n", errno);
        throw;
    }

    while(true) {
        // Get data from client
        std::cout << "Listening on port " << config.getListeningPort() << std::endl; 
        int clientSocket = accept(socketDescriptor, nullptr, nullptr);

        char buffer[1024] = { 0 };
        uint64_t receiveSize = recv(clientSocket, buffer, sizeof(buffer), 0);
        std::cout << "Message from client: " << buffer << std::endl;
        
        std::string returnMessage = "KAGEM;AMD";
        if(config.hasPeers()) {
            // send request to next peer and get a response
            IpAddress address = config.getNextRemoteAddress();
            sendMessage(socketDescriptor, address, std::string { buffer, receiveSize });
            
            receiveSize = recv(socketDescriptor, buffer, sizeof(buffer), 0);
            returnMessage.append(buffer, receiveSize);
            std::cout << "Message from peer: " << returnMessage << std::endl;
        }

        // send response back to the client and close socket
        std::cout << "Senda to client\n";
        int ret = send(clientSocket, returnMessage.data(), returnMessage.size(), 0);

        if(ret == -1) {
            printf("Failed to send message back to client: %d \n", errno);
            throw;
        }

        std::cout << "CLosing socket " << std::to_string(ret) << " \n";
        close(clientSocket);
    }

    close(socketDescriptor);
}

