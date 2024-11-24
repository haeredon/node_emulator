#ifndef RECEIVER_H
#define RECEIVER_H

#include "Node.h"

class Receiver : public Node {
        
    private:

        int listeningPort;

    public: 

        Receiver(std::string&& tag, std::vector<Peer>&& peers, int listeningPort);

        void start() override;

};





#endif // RECEIVER_H