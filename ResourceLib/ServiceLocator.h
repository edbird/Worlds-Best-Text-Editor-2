#ifndef SERVICE_LOCATOR_H
#define SERVICE_LOCATOR_H


// Internal includes
//#include "SDLManager.h"
#include "SDLResourceManager.h"

// C++ includes
#include <memory>


class ServiceLocator
{

public:

    ServiceLocator();

    //static std::shared_ptr<SDLManager> getSDLManager();

    static std::shared_ptr<SDLResourceManager> getSDLResourceManager();


private:

    //static std::shared_ptr<SDLManager> sdlManager;

    static std::shared_ptr<SDLResourceManager> sdlResourceManager;

};


#endif // SERVICE_LOCATOR_H