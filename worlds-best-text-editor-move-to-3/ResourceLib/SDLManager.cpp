#include "SDLManager.h"


// Internal includes
#include "ExceptionTypes.h"

// External includes
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// C++ includes
#include <memory>
#include <iostream>


static SDLManager gSDLManager;


SDLManager::SDLManager()
{
    // standard SDL init sequence
    try
    {
        libsInit();
    }
    catch(const SDLLibException &error)
    {
        throw;
    }
    catch(const TTFLibException &error)
    {
        throw;
    }
}


void SDLManager::libsInit()
{   
    if(SDL_WasInit(SDL_INIT_VIDEO))
    {
        // NOOP
    }
    else
    {
        if(SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            std::cerr << SDL_GetError() << std::endl;

            SDL_Quit(); // not sure if required

            throw SDLLibException("SDL_Init error");
        }
    }

    if(TTF_WasInit())
    {
        // NOOP
    }
    else
    {
        if(TTF_Init() < 0)
        {
            std::cerr << TTF_GetError() << std::endl;

            TTF_Quit(); // not sure if required
            SDL_Quit();

            throw TTFLibException("TTF_Init error");
        }
    }
}


bool SDLManager::initSuccess()
{
    return SDL_WasInit(SDL_INIT_VIDEO) && TTF_WasInit();
}


void SDLManager::libsDestroy()
{
    if(TTF_WasInit())
    {
        TTF_Quit();
    }

    if(SDL_WasInit(SDL_INIT_VIDEO))
    {
        SDL_Quit();
    }
}



