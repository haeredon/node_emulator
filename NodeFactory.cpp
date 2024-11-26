#include "NodeFactory.h"
#include "Peer.h"
#include "Requester.h"
#include "Receiver.h"
#include "Node.h"

#include <unordered_map>
#include <iostream>
#include <fstream>
#include <algorithm>

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

    double aggAffinity = 0;
    for(auto& peer : peers) {
        aggAffinity += peer.getAffinity();
        peer.setAffinity(aggAffinity);
    }

    return peers;
}



void validateRequestConfig(std::unordered_map<std::string, std::string>& configs) {
    std::string tag = configs.contains("tag") ? configs.at("tag") : "";
    uint16_t timeBetweenRequests  = configs.contains("timeBetweenRequests") ? stoi(configs.at("timeBetweenRequests")) : 0;
    std::string peers  = configs.contains("peers") ? configs.at("peers") : "";

    if(tag.size() == 0 || timeBetweenRequests == 0 || peers.size() == 0) {
        std::cout << "Invalid Requester Configuration" << std::endl;
        throw;
    } 
}

void validateReceiverConfig(std::unordered_map<std::string, std::string>& configs) {
    std::string tag = configs.contains("tag") ? configs.at("tag") : "";
    uint16_t listeningPort  = configs.contains("listeningPort") ? stoi(configs.at("listeningPort")) : 0;
    std::string peers  = configs.contains("peers") ? configs.at("peers") : "";

    if(tag.size() == 0 || listeningPort == 0) {
        std::cout << "Invalid Receiver Configuration" << std::endl;
        throw;
    } 
}

/*************************** PUBLIC INTERFACE *************************************************/

Node* NodeFactory::createNode(std::string configFile) {
    std::cout << "Executable: " << configFile << std::endl; 

    std::unordered_map<std::string, std::string> config = getConfigs(configFile);
    const std::string nodeTypeKey = "NODE_TYPE";

    if(config.contains(nodeTypeKey)) {
        std::string nodeType = config.at(nodeTypeKey);
        
        if(nodeType == "REQUESTER") {
            validateRequestConfig(config);            
            return new Requester {
                std::move(config.at("tag")),
                createPeerMappings(config["peers"]),
                stoi(config.at("timeBetweenRequests"))
            };
        } else if(nodeType == "RECEIVER") {
            validateReceiverConfig(config);            
            return new Receiver {
                std::move(config.at("tag")),
                createPeerMappings(config["peers"]),
                stoi(config.at("listeningPort"))
            };
        } else {
            std::cout << "Invalid NODE_TYPE" << std::endl;
            throw;
        }
    } else {
        std::cout << "NODE_TYPE not specified" << std::endl;
        throw;
    }
}
