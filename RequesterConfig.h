#ifndef REQUESTER_CONFIG_H
#define REQUESTER_CONFIG_H

#include "Config.h"
#include "Peer.h"

#include <vector>
#include <string>


class RequesterConfig : public Config {

    private: 

        int timeBetweenRequests;

    public:

        RequesterConfig(std::string tag, int timeBetweenRequests, std::vector<Peer>&& peers);

        int getTimeBetweenRequests();

        std::string toString() override;

};




#endif // REQUESTER_CONFIG_H