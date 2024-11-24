#ifndef IP_ADDRESS_H
#define IP_ADDRESS_H


#include <unistd.h>
#include <string>

class IpAddress {

    private:

        uint32_t ip;
        uint16_t port;

    public:

        IpAddress(uint32_t ip, uint16_t port);

        uint32_t getIp();

        uint32_t getPort();

        std::string toString() const;

};




#endif // IP_ADDRESS_H