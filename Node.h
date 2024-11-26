#ifndef NODE_H
#define NODE_H

#include "Hook.h"
#include "IpAddress.h"
#include "Peer.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <vector>
#include <iostream>

class Node {

    protected:

        std::vector<Hook*> hooks;

        std::string tag;

        std::vector<Peer> peers;

        bool started;

        const IpAddress& getNextRemoteAddress() {
            double choice = (((double) rand()) / (double) RAND_MAX);

            for(const Peer& peer : peers) {
                if(choice < peer.getAffinity()) {
                    return peer.getAddress();
                }
            }
            throw;
        }

        int getSocket() {
            int socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);

            if(socketDescriptor == -1) {
                printf("Failed to initialize socket: %d \n", errno);
                throw;
            }

            return socketDescriptor;
        }   

        struct sockaddr_in getAddrSpec(uint32_t ip, uint16_t port) {
            struct sockaddr_in addr = {
                .sin_family = AF_INET,
                .sin_port = htons(port),
                .sin_addr = {
                    .s_addr = ip
                }
            };    

            return addr;
        }

        void socketConnect(int socketDescriptor, const struct sockaddr *addr, socklen_t addrlen) {
            int ret = connect(socketDescriptor, addr, addrlen);

            if(ret == -1) {
                printf("Failed to connect to remote server: %d \n", errno);
                throw;
            }
        }

        std::string sendMessage(int socketDescriptor, IpAddress& address, std::string message) {
            const struct sockaddr_in addr = getAddrSpec(address.getIp(), address.getPort());
            socketConnect(socketDescriptor, (struct sockaddr*) &addr, sizeof(addr));

            // sending data    
            int ret = send(socketDescriptor, message.data(), message.size(), 0);
            
            // receiving data
            char buffer[1024] = { 0 };
            size_t receiveSize = recv(socketDescriptor, buffer, sizeof(buffer), 0);
            
            if(ret == -1) {
                printf("Failed to send data to remote server: %d \n", errno);
                throw;
            }

            return std::string { buffer, receiveSize };
        }
        
    public:

        Node(std::string&& tag, std::vector<Peer>&& peers) : 
            peers(std::forward<std::vector<Peer>>(peers)),
            tag(std::forward<std::string>(tag)) { }

        virtual void start() = 0;
        
        void addHook(Hook* hook) {
            hooks.push_back(hook);
        }

        bool hasStarted() {
            return this->started;
        }
};






#endif // NODE_H