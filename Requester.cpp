#include "Requester.h"


Requester::Requester(std::string&& tag, std::vector<Peer>&& peers, int timeBetweenRequests) : 
    Node(std::forward<std::string>(tag), std::forward<std::vector<Peer>>(peers)), timeBetweenRequests(timeBetweenRequests) { }


void Requester::start() {  
    while(true) {        
        int socketDescriptor = getSocket();

        IpAddress address = getNextRemoteAddress();
        sendMessage(socketDescriptor, address, tag + " calling.\n");

        close(socketDescriptor);

        sleep(timeBetweenRequests);
    }    
}
