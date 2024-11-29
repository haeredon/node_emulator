#ifndef HOOK_H
#define HOOK_H


#include <string>

enum class HOOK_TYPE { 
    NETWORK_PRE_SEND,
    NETWORK_POST_SEND,
    NETWORK_PRE_RECEIVE,
    NETWORK_POST_RECEIVE
 };

class Hook {
    
    public:
                
        virtual void event(HOOK_TYPE hookType, std::string& data) = 0;

};






#endif // HOOK_H