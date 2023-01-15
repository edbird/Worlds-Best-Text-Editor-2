#include "SDLResourceManager.h"

// Internal includes
#include "ExceptionTypes.h"
#include "ServiceLocator.h"
#include "SDLManager.h"

// External includes
#include <SDL2/SDL.h>


// TODO: reference here?
SDLResourceManager::SDLResourceManager()
    : mDefaultWindowSizeX(DEFAULT_WINDOW_SIZE_X)
    , mDefaultWindowSizeY(DEFAULT_WINDOW_SIZE_Y)
    , mNextWindowId(0)
{
    // TODO: it probably shouldn't be an error to construct this class, only use it
    if(SDLManager::initSuccess())
    {
        // Do nothing
    }
    else
    {
        throw TTFLibException(
            "Error in SDLResourceManager(), TTF library previously failed to initialize");
    }
}


// TODO: prevent exceptions emitting from destructor
SDLResourceManager::~SDLResourceManager()
{
    if(SDLManager::initSuccess())
    {
        destroyAllWindows();
    }
}




bool SDLResourceManager::windowCreated(const windowId_t windowId) const
{
    if(windowId != 0)
    {
        throw std::runtime_error("window id");
    }

    return static_cast<bool>(mapWindows.at(windowId).window);
}


void SDLResourceManager::destroyWindow(const windowId_t windowId)
{
    auto window(mapWindows.at(windowId));

    if(auto pRenderer = window.renderer)
    {
        pRenderer.reset();
    }

    if(auto pWindow = window.window)
    {
        pWindow.reset();
    }
}


void SDLResourceManager::destroyAllWindows()
{
    for(auto [windowId, window] : mapWindows)
    {
        destroyWindow(windowId);
    }
}


std::shared_ptr<SDL_Window> SDLResourceManager::getWindow(const windowId_t windowId)
{
    if(windowId != 0)
    {
        throw std::runtime_error("windowId");
    }

    const auto window(mapWindows.at(windowId).window);

    if(!window)
    {
        throw std::runtime_error("window invalid");
    }

    return window;
}


std::shared_ptr<SDL_Renderer> SDLResourceManager::getWindowRenderer(const windowId_t windowId)
{
    if(windowId != 0)
    {
        throw std::runtime_error("windowId");
    }

    const auto renderer(mapWindows.at(windowId).renderer);

    if(!renderer)
    {
        throw std::runtime_error("renderer invalid");
    }

    return renderer;
}


windowId_t SDLResourceManager::createWindow()
{
    std::shared_ptr<SDL_Window> window;
    return createWindow(window);
}


windowId_t SDLResourceManager::createWindow(std::shared_ptr<SDL_Window>& window)
{
    const std::string windowTitle("SDL Text Graphics Library (C) Ed Bird 2021");

    const unsigned short width = mDefaultWindowSizeX;
    const unsigned short height = mDefaultWindowSizeY;

    return createWindow(window, windowTitle, width, height);
}


windowId_t SDLResourceManager::createWindow(
    const std::string& windowTitle,
    const unsigned short width,
    const unsigned short height)
{
    std::shared_ptr<SDL_Window> window;
    return createWindow(window, windowTitle, width, height);
}


windowId_t SDLResourceManager::createWindow(
    std::shared_ptr<SDL_Window>& window,
    const std::string& windowTitle,
    const unsigned short width,
    const unsigned short height)
{
    windowId_t windowId{0};

    if(SDLManager::initSuccess())
    {
        while(mapWindows.count(mNextWindowId))
        {
            ++ mNextWindowId;
        }

        windowId = mNextWindowId;
        ++ mNextWindowId;

        if(!mapWindows.count(windowId))
        {
            //std::shared_ptr<SDL_Window> window;
            window.reset(
                SDL_CreateWindow(
                    windowTitle.c_str(),
                    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                    width, height,
                    SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE),
                SDL_DestroyWindow);

            if(!window)
            {
                throw SDLLibException(
                    "Error: failed to create window");
            }
            else
            {
                std::shared_ptr<SDL_Renderer> renderer;
                renderer.reset(
                    SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED),
                    SDL_DestroyRenderer);

                if(!renderer)
                {
                    throw SDLLibException(
                        "Error: failed to create renderer associated to window");
                }
                else
                {
                    // Store window and renderer pointer in map
                    mapWindows[windowId] = WindowData{
                        .window = window,
                        .renderer = renderer
                    };
                }
                
                // return the successfully created window regardless
                // of what happened when initializing the renderer
                //return windowId;
            }
        }
        else
        {
            std::string message(
                std::string("Window with id ")
                + std::to_string(windowId)
                + " already created"
            );
            throw SDLLibException(message);
        }
    }
    else
    {
        throw SDLLibException("Error: CreateWindow: SDL lib did not initialize correctly");
    }

    return windowId;
}






    ////////////////////////////////////////////////////////////////////////////
    // Old window creation API
    ////////////////////////////////////////////////////////////////////////////
#if 0
    void window_init(const SDLManager &manager)
    {
        if(manager.SDLInitSuccess())
        {
            if(m_init_success)
            {
                std::cout << "Window Size: " << m_window_size_x << " "
                        << m_window_size_y << std::endl;

                m_window.reset(
                    SDL_CreateWindow(
                        "SDL Text Graphics Library",
                        SDL_WINDOWPOS_UNDEFINED,
                        SDL_WINDOWPOS_UNDEFINED,
                        m_window_size_x, m_window_size_y,
                        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE),
                    SDL_DestroyWindow);

                if(m_window.get() == nullptr)
                {
                    std::cerr << SDL_GetError() << std::endl;

                    //libs_destroy();
                    throw SDLLibException("Error in window_init(), could not create SDL window object");
                }
                else
                {
                    m_window_init_success = true;
                }
            }
            else
            {
                throw SDLLibException("Error in window_init(), could not create SDL window object, window already initialized");
            }
        }
        else
        {
            throw SDLLibException("Error in window_init(), SDL library is not initialized");
        }
    }


    void window_destroy()
    {
        if(m_window_init_success)
        {
            m_window_init_success = false;
            m_window.reset();
        }
        else
        {
            // ignore
        }
    }


    void renderer_init(const SDLManager &manager)
    {
        if(manager.SDLInitSuccess())
        {
            if(m_window_init_success)
            {
                m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
                if(m_renderer == nullptr)
                {
                    std::cerr << SDL_GetError() << std::endl;

                    window_destroy();
                    libs_destroy();

                    throw SDLLibException("Error in renderer_init(), could not create SDL renderer object");
                }
                else
                {
                    m_renderer_init_success = true;
                }
            }
            else
            {
                throw SDLLibException("Error in renderer_init(), previous error caused SDL init or window init to fail");
            }
        }
        else
        {
            throw SDLLibException("Error in window_init(), SDL library is not initialized");
        }
    }


    void renderer_destroy()
    {
        #if 0
        if(m_renderer_init_success)
        {
            m_renderer_init_success = false;
            m_renderer.reset();
        }
        else
        {
            // ignore
        }
        #endif

        m_renderer.reset();
    }
#endif // #if 0