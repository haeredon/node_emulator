#ifndef RECEIVER_CONFIG_H
#define RECEIVER_CONFIG_H

#include "Config.h"
#include "Peer.h"

#include <string>
#include <vector>


class ReceiverConfig : public Config  {

    private:

        int listeningPort;
    
    public:

        ReceiverConfig(std::string tag, int listeningPort, std::vector<Peer>&& peers);

        int getListeningPort();

        std::string toString() override;
};






#endif // RECEIVER_CONFIG_H