#ifndef RECEIVER_H
#define RECEIVER_H

#include "Node.h"
#include "ReceiverConfig.h"

class Receiver : public Node<ReceiverConfig> {
        
    public: 

        Receiver(ReceiverConfig config);

        void start() override;

};





#endif // RECEIVER_H