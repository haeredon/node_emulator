#ifndef REQUESTER_H
#define REQUESTER_H

#include "Node.h"

class Requester : public Node {
        
    private:

        int timeBetweenRequests;

    public: 

        Requester(std::string&& tag, std::vector<Peer>&& peers, int timeBetweenRequests);

        void start() override;

};





#endif // REQUESTER_H