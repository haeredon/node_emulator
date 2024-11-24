#include "IpAddress.h"

#include <arpa/inet.h>

IpAddress::IpAddress(uint32_t ip, uint16_t port) : ip(ip), port(port) {}

uint32_t IpAddress::getIp() {
    return ip;
}

uint32_t IpAddress::getPort() {
    return port;
}

std::string IpAddress::toString() const {
    std::string print = "IpAddress \n\t";
    print
    .append("ip: ").append(inet_ntoa((struct in_addr) ip)).append("\n\t")
    .append("port: ").append(std::to_string(port)).append("\n");

    return print;
};