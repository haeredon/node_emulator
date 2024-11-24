#include "Config.h"


Config::Config(std::string tag, std::vector<Peer>&& peers) : tag(tag), peers(std::forward<std::vector<Peer>>(peers)) {
    srand(time(0));
}

std::string Config::getTag() {
    return tag;
}

bool Config::hasPeers() {
    return !peers.empty();
}

const IpAddress& Config::getNextRemoteAddress() {
    double choice = (rand() / RAND_MAX);

    for(const Peer& peer : peers) {
        if(choice < peer.getAffinity()) {
            return peer.getAddress();
        }
    }
    throw;
}
