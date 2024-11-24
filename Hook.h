#ifndef HOOK_H
#define HOOK_H


#include <string>

enum class HOOK_TYPE { sdfddas };

class Hook {
    
    public:

        virtual void event(HOOK_TYPE hookType, void* data);

};






#endif // HOOK_H