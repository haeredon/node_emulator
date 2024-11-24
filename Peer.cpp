#include "Peer.h"
#include "IpAddress.h"

Peer::Peer(double affinity, IpAddress address) : affinity(affinity), address(address) {}

const IpAddress& Peer::getAddress() const {
    return address;
}

double Peer::getAffinity() const {
    return affinity;
}

std::string Peer::toString() const {
    std::string print = "Peer \n";
    print
    .append("affinity: ").append(std::to_string(affinity)).append("\n\t")
    .append(address.toString());

    return print;
};
