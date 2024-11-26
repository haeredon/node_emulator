#include "Requester.h"


Requester::Requester(std::string&& tag, std::vector<Peer>&& peers, int timeBetweenRequests) : 
    Node(std::forward<std::string>(tag), std::forward<std::vector<Peer>>(peers)), timeBetweenRequests(timeBetweenRequests) { }


void Requester::start() {  
    this->started = true; 
    while(true) {        
        int socketDescriptor = getSocket();

        IpAddress address = getNextRemoteAddress();
        std::string response = sendMessage(socketDescriptor, address, tag + " calling.\n");

        std::cout << "Message from Server: " << response << std::endl;

        close(socketDescriptor);

        sleep(timeBetweenRequests);
    }    
}
