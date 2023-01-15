#include "ServiceLocator.h"

// Internal includes
#include "SDLManager.h"
#include "SDLResourceManager.h"

// C++ includes
#include <memory>


//static std::shared_ptr<SDLManager> ServiceLocator::sdlManager(nullptr);

std::shared_ptr<SDLResourceManager> ServiceLocator::sdlResourceManager(nullptr);


ServiceLocator::ServiceLocator()
{
}


/*static std::shared_ptr<SDLManager> ServiceLocator::getSDLManager()
{    
    if(!sdlManager)
    {
        sdlManager = std::make_shared<SDLManager>();
    }
    
    return sdlManager;
}*/


std::shared_ptr<SDLResourceManager> ServiceLocator::getSDLResourceManager()
{    
    if(!sdlResourceManager)
    {
        sdlResourceManager = std::make_shared<SDLResourceManager>();
    }

    return sdlResourceManager;
}

