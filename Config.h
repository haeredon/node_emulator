#ifndef CONFIG_H
#define CONFIG_H

#include "Peer.h"
#include "IpAddress.h"

#include <string>
#include <vector>
#include <iostream>

class Config {

    protected: 

        std::string tag;

        std::vector<Peer> peers;

    public:

        Config(std::string tag, std::vector<Peer>&& peers);

        std::string getTag();

        bool hasPeers();

        const IpAddress& getNextRemoteAddress();

        virtual std::string toString() = 0;
};


#endif // CONFIG_H