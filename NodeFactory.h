#ifndef NODE_FACTORY_H
#define NODE_FACTORY_H


#include <string>
#include <vector>
#include "Hook.h"

class NodeFactory {

    private:

        std::vector<Hook*> hooks;
    
    public:

        static void createNode(std::string configFile);

        void addHook(Hook* hook);

};






#endif // NODE_FACTORY_H