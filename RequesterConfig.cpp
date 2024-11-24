#include "RequesterConfig.h"




RequesterConfig::RequesterConfig(std::string tag, int timeBetweenRequests, std::vector<Peer>&& peers) : Config(tag, std::forward<std::vector<Peer>>(peers)), timeBetweenRequests(timeBetweenRequests) {}

int RequesterConfig::getTimeBetweenRequests() {
    return timeBetweenRequests;
}    

std::string RequesterConfig::toString() {
    std::string print = "RequesterConfig \n\t";
    print
    .append("Tag: ").append(tag).append("\n\t")
    .append("TimeBetweenRequests: ").append(std::to_string(timeBetweenRequests)).append("\n\t")
    .append("Peers: ").append("\n\t\t");

    for(const Peer& peer : peers) {
        print.append(peer.toString()).append("\n\t\t");
    }

    return print;
};
