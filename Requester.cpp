#include "Requester.h"


Requester::Requester(RequesterConfig config) : Node(std::forward<RequesterConfig>(config)) { }


void Requester::start() {  
    while(true) {        
        int socketDescriptor = getSocket();

        IpAddress address = config.getNextRemoteAddress();
        sendMessage(socketDescriptor, address, config.getTag().append(" calling.\n"));

        close(socketDescriptor);

        sleep(config.getTimeBetweenRequests());
    }    
}
