#include "ReceiverConfig.h"


ReceiverConfig::ReceiverConfig(std::string tag, int listeningPort, std::vector<Peer>&& peers) : Config(tag, std::forward<std::vector<Peer>>(peers)), listeningPort(listeningPort) {}

int ReceiverConfig::getListeningPort() {
    return listeningPort;
}

std::string ReceiverConfig::toString() {
    std::string print = "ReceiverConfig \n\t";
    print
    .append("Tag: ").append(tag).append("\n\t")
    .append("listeningPort: ").append(std::to_string(listeningPort)).append("\n\t")
    .append("Peers: ").append("\n\t\t");

    for(const Peer& peer : peers) {
        print.append(peer.toString()).append("\n\t\t");
    }

    return print;
};

