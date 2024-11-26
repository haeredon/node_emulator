#ifndef IP_PEER_H
#define IP_PEER_H

#include "IpAddress.h"

#include <string>


class Peer {

    private:

        double affinity;

        IpAddress address;
        
    public: 

        Peer(double affinity, IpAddress address);

        const IpAddress& getAddress() const;

        double getAffinity() const;

        void setAffinity(double affinity);

        std::string toString() const;

};





#endif // IP_PEER_H