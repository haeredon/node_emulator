#include "Receiver.h"
#include "Peer.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


Receiver::Receiver(std::string&& tag, std::vector<Peer>&& peers, int listeningPort) : 
    Node(std::forward<std::string>(tag), std::forward<std::vector<Peer>>(peers)), listeningPort(listeningPort) { }


void Receiver::start() { 
    int socketDescriptor = getSocket();

    const struct sockaddr_in addr = getAddrSpec(INADDR_ANY, listeningPort);

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

    std::cout << "Listening on port " << std::to_string(listeningPort) << std::endl; 

    this->started = true; 
    while(true) {
        // Get data from client        
        int clientSocket = accept(socketDescriptor, nullptr, nullptr);

        char buffer[1024] = { 0 };
        uint64_t receiveSize = recv(clientSocket, buffer, sizeof(buffer), 0);
        std::cout << "Message from client: " << buffer << std::endl;
        
        std::string response = "";
        if(peers.empty()) {
            response = this->tag + " responding";
        } else {
            // send request to next peer and get a response
            int peerSocket = getSocket();            
            IpAddress address = getNextRemoteAddress();
            response = sendMessage(peerSocket, address, std::string { buffer, receiveSize });
            
            close(peerSocket);
        }

        // send response back to the client and close socket
        int ret = send(clientSocket, response.data(), response.size(), 0);

        if(ret == -1) {
            printf("Failed to send message back to client: %d \n", errno);
            throw;
        }

        close(clientSocket);
    }

    close(socketDescriptor);
}

