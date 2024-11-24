#ifndef REQUESTER_H
#define REQUESTER_H

#include "Node.h"
#include "RequesterConfig.h"

class Requester : public Node<RequesterConfig> {
        
    public: 

        Requester(RequesterConfig config);

        void start() override;

};





#endif // REQUESTER_H