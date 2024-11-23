
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <cstdlib>
#include <algorithm>
#include <regex>

class IpAddress {

    private:

        uint32_t ip;
        uint16_t port;

    public:

        IpAddress(uint32_t ip, uint16_t port) : ip(ip), port(port) {}

        uint32_t getIp() {
            return ip;
        }

        uint32_t getPort() {
            return port;
        }

        std::string toString() const {
            std::string print = "IpAddress \n\t";
            print
            .append("ip: ").append(inet_ntoa((struct in_addr) ip)).append("\n\t")
            .append("port: ").append(std::to_string(port)).append("\n");

            return print;
        };

};

class Peer {

    private:

        double affinity;

        IpAddress address;
        
    public: 

        Peer(double affinity, IpAddress address) : affinity(affinity), address(address) {}

        const IpAddress& getAddress() const {
            return address;
        }

        double getAffinity() const {
            return affinity;
        }

        std::string toString() const {
            std::string print = "Peer \n";
            print
            .append("affinity: ").append(std::to_string(affinity)).append("\n\t")
            .append(address.toString());

            return print;
        };

};

class Config {

    protected: 

        std::string tag;

        std::vector<Peer> peers;

    public:

        Config(std::string tag, std::vector<Peer>&& peers) : tag(tag), peers(std::forward<std::vector<Peer>>(peers)) {
            srand(time(0));
        }

        std::string getTag() {
            return tag;
        }

        bool hasPeers() {
            return !peers.empty();
        }

        const IpAddress& getNextRemoteAddress() {
            double choice = (rand() / RAND_MAX);

            for(const Peer& peer : peers) {
                if(choice < peer.getAffinity()) {
                    return peer.getAddress();
                }
            }
            throw;
        }

        virtual std::string toString() = 0;
};

class RequesterConfig : public Config {

    private: 

        int timeBetweenRequests;

    public:

        RequesterConfig(std::string tag, int timeBetweenRequests, std::vector<Peer>&& peers) : Config(tag, std::forward<std::vector<Peer>>(peers)), timeBetweenRequests(timeBetweenRequests) {}

        int getTimeBetweenRequests() {
            return timeBetweenRequests;
        }    

        std::string toString() override {
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

};

class ReceiverConfig : public Config  {

    private:

        int listeningPort;
    
    public:

        ReceiverConfig(std::string tag, int listeningPort, std::vector<Peer>&& peers) : Config(tag, std::forward<std::vector<Peer>>(peers)), listeningPort(listeningPort) {}

        int getListeningPort() {
            return listeningPort;
        }

        std::string toString() override {
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
        
};


int getSocket() {
    int socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);

    if(socketDescriptor == -1) {
        printf("Failed to initialize socket: %d \n", errno);
        throw;
    }

    return socketDescriptor;
}

struct sockaddr_in getAddrSpec(uint32_t ip, uint16_t port) {
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr = {
            .s_addr = ip
        }
    };    

    return addr;
}

void socketConnect(int socketDescriptor, const struct sockaddr *addr, socklen_t addrlen) {
    int ret = connect(socketDescriptor, addr, addrlen);

    if(ret == -1) {
        printf("Failed to connect to remote server: %d \n", errno);
        throw;
    }
}

void sendMessage(int socketDescriptor, IpAddress& address, std::string message) {
    const struct sockaddr_in addr = getAddrSpec(address.getIp(), address.getPort());
    socketConnect(socketDescriptor, (struct sockaddr*) &addr, sizeof(addr));

    // sending data    
    int ret = send(socketDescriptor, message.data(), message.size(), 0);
    
    // receiving data
    char buffer[1024] = { 0 };
    size_t receiveSize = recv(socketDescriptor, buffer, sizeof(buffer), 0);
    std::cout << "Message from server: " << std::string { buffer, receiveSize } << std::endl;


    std::cout << "Message from server length: " << std::to_string(receiveSize) << std::endl;

    if(ret == -1) {
        printf("Failed to send data to remote server: %d \n", errno);
        throw;
    }
}


void startRequester(RequesterConfig& config) {  
    while(true) {        
        int socketDescriptor = getSocket();

        IpAddress address = config.getNextRemoteAddress();
        sendMessage(socketDescriptor, address, config.getTag().append(" calling.\n"));

        close(socketDescriptor);

        sleep(config.getTimeBetweenRequests());
    }    
}


void startReceiver(ReceiverConfig& config) {  
    int socketDescriptor = getSocket();

    const struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(config.getListeningPort()),
        .sin_addr = {
            .s_addr = INADDR_ANY
        }
    };    

    int ret = bind(socketDescriptor, (struct sockaddr*) &addr, sizeof(addr));

    if(ret == -1) {
        printf("Failed to bind socket: %d \n", errno);
        throw;
    }

    ret = listen(socketDescriptor, 10 /* backlog */);

    if(ret == -1) {
        printf("Failed to listen on socket: %d \n", errno);
        throw;
    }

    while(true) {
        // Get data from client
        std::cout << "Listening on port " << config.getListeningPort() << std::endl; 
        int clientSocket = accept(socketDescriptor, nullptr, nullptr);

        char buffer[1024] = { 0 };
        uint64_t receiveSize = recv(clientSocket, buffer, sizeof(buffer), 0);
        std::cout << "Message from client: " << buffer << std::endl;
        
        std::string returnMessage = "KAGEM;AMD";
        if(config.hasPeers()) {
            // send request to next peer and get a response
            IpAddress address = config.getNextRemoteAddress();
            sendMessage(socketDescriptor, address, std::string { buffer, receiveSize });
            
            receiveSize = recv(socketDescriptor, buffer, sizeof(buffer), 0);
            returnMessage.append(buffer, receiveSize);
            std::cout << "Message from peer: " << returnMessage << std::endl;
        }

        // send response back to the client and close socket
        std::cout << "Senda to client\n";
        int ret = send(clientSocket, returnMessage.data(), returnMessage.size(), 0);

        if(ret == -1) {
            printf("Failed to send message back to client: %d \n", errno);
            throw;
        }

        std::cout << "CLosing socket " << std::to_string(ret) << " \n";
        close(clientSocket);
    }

    close(socketDescriptor);
}

std::unordered_map<std::string, std::string> getConfigs(std::string filePath) {
    std::unordered_map<std::string, std::string> configs;

    std::ifstream configFile(filePath);

    std::string configuration;

    while(configFile.good() && std::getline(configFile, configuration)) {
        std::cout << configuration << std::endl;

        size_t equalIdx = configuration.find_first_of('=');
        size_t commentIdx = configuration.find_first_of('#');

        if(commentIdx == 0) {
            continue;
        }

        if(equalIdx == std::string::npos) {
            std::cout << "Missing '=' in configuration file" << std::endl;    
        }

        std::string key = configuration.substr(0, equalIdx);
        std::string value = configuration.substr(equalIdx + 1, std::string::npos);

        // remove spaces
        std::string::iterator key_end_pos = std::remove(key.begin(), key.end(), ' ');
        key.erase(key_end_pos, key.end());
        std::string::iterator value_end_pos = std::remove(value.begin(), value.end(), ' ');
        value.erase(value_end_pos, value.end());

        configs.insert(std::pair<std::string, std::string>(key, value));
    }

    if(!configFile.eof()) {
        std::cout << "Failed to read config file: " << std::to_string(errno) << std::endl;
        throw;
    }

    configFile.close(); 

    return configs;
}


std::vector<Peer> createPeerMappings(std::string peerString) {
    std::vector<Peer> peers; 

    std::string tempStr = peerString;
    size_t firstCommaIdx = tempStr.find_first_of(',');
    while(tempStr.size() > 0) {
        size_t ipPortDividerIdx = tempStr.find_first_of(':');
        size_t affinityDividerIdx = tempStr.find_last_of(':');

        if((ipPortDividerIdx == std::string::npos || affinityDividerIdx == std::string::npos)
            && ipPortDividerIdx != affinityDividerIdx) {
            std::cout << "Invalid Peer Address" << std::endl;    
        }
        
        uint32_t ip =  inet_addr(tempStr.substr(0, ipPortDividerIdx).data());
        uint16_t port = stoi(tempStr.substr(ipPortDividerIdx + 1, std::string::npos));
        double affinity = stod(tempStr.substr(affinityDividerIdx + 1, std::string::npos));

        peers.emplace_back(affinity, IpAddress { ip, port });

        tempStr.erase(0, firstCommaIdx == std::string::npos ? std::string::npos : firstCommaIdx + 1);
        firstCommaIdx = tempStr.find_first_of(',');
    }

    std::sort(peers.begin(), peers.end(), [](const Peer& a, const Peer& b) { 
            return a.getAffinity() < b.getAffinity(); 
        }
    );

    return peers;
}



RequesterConfig buildRequestConfig(std::unordered_map<std::string, std::string>& configs) {
    std::string tag = configs.contains("tag") ? configs.at("tag") : "";
    uint16_t timeBetweenRequests  = configs.contains("timeBetweenRequests") ? stoi(configs.at("timeBetweenRequests")) : 0;
    std::string peers  = configs.contains("peers") ? configs.at("peers") : "";

    if(tag.size() == 0 || timeBetweenRequests == 0) {
        std::cout << "Invalid Receiver Configuration" << std::endl;
        throw;
    } 
    
    return RequesterConfig { tag, timeBetweenRequests, createPeerMappings(peers)};
}

ReceiverConfig buildReceiverConfig(std::unordered_map<std::string, std::string>& configs) {
    std::string tag = configs.contains("tag") ? configs.at("tag") : "";
    uint16_t listeningPort  = configs.contains("listeningPort") ? stoi(configs.at("listeningPort")) : 0;
    std::string peers  = configs.contains("peers") ? configs.at("peers") : "";

    if(tag.size() == 0 || listeningPort == 0) {
        std::cout << "Invalid Receiver Configuration" << std::endl;
        throw;
    } 
    
    return ReceiverConfig { tag, listeningPort, createPeerMappings(peers) };
}


int main(int argc, char* argv[]) {
    std::cout << "Executable: " << argv[0] << std::endl; 

    if(argc == 2) {
        std::string configPath{ argv[1] };

        std::cout << "Reading config from: " << configPath << std::endl;

        std::unordered_map<std::string, std::string> configs = getConfigs(configPath);
        const std::string nodeTypeKey = "NODE_TYPE";

        if(configs.contains(nodeTypeKey)) {
            std::string nodeType = configs.at(nodeTypeKey);
            
            if(nodeType == "REQUESTER") {
                RequesterConfig config = buildRequestConfig(configs);
                std::cout << "Running with config: \n" << config.toString() << std::endl;
                startRequester(config);
            } else if(nodeType == "RECEIVER") {
                ReceiverConfig config = buildReceiverConfig(configs);
                std::cout << "Running with config: \n" << config.toString() << std::endl;
                startReceiver(config);
            } else {
                std::cout << "Invalid NODE_TYPE" << std::endl;
                return -1;
            }
        } else {
            std::cout << "NODE_TYPE not specified" << std::endl;
            return -1;
        }
        
    } else {
        std::cout << "Bad Arguments" << std::endl;
        return -1;
    }

    return 0;
    
}