#ifndef NODE_H
#define NODE_H


#include <string>
#include <vector>
#include "Hook.h"


class Node {

    private:

        std::vector<Hook*> hooks;


        
    
    public:

        static void start();

        void addHook(Hook* hook);

};






#endif // NODE_H