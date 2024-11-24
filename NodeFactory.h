#ifndef NODE_FACTORY_H
#define NODE_FACTORY_H


#include <string>
#include <vector>
#include "Hook.h"
#include "Node.h"

class NodeFactory {

    private:

        std::vector<Hook*> hooks;
    
    public:

        template<class TEST>
        static Node* createNode(std::string configFile);

};






#endif // NODE_FACTORY_H