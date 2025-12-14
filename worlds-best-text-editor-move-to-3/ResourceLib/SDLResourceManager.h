#ifndef SDL_RESOURCE_MANAGER_H
#define SDL_RESOURCE_MANAGER_H


// TODO: add a create_window() function to the singleton
// class and have this class manage creation / destruction
// of the windows
// the singleton can then check if SDL/TTF was init correctly
// before creating a window
// function should return a shared pointer to a window?

// TODO: clean this class up, it contains some old code
// which was present before it was split out into the seperate
// SDLManager class

// TODO: should this class be able to create and destory multiple windows?
// or should there be one of these classes per window?
// probably the former, else this class is just a C++ wrapper around some
// SDL2 C code
// which is essentially the same API as SFML


// Internal includes
#include "ExceptionTypes.h"

// External includes
#include <SDL2/SDL.h>

// C++ includes
#include <string>
#include <map>
#include <memory>


// TODO: namespace scope / class scope
typedef int windowId_t;


// Called a resource manager, but it only manages SDL_Windows and SDL_Renderers

// this class manages a single SDL window
// TODO: update so that multiple windows are supported
class SDLResourceManager
{

    // Manages creation and destruction of SDL_Window objects

public:

    // TODO: reference here?
    SDLResourceManager();

    ~SDLResourceManager();

    bool windowCreated(const windowId_t windowId) const;

    void destroyWindow(const windowId_t windowId);

    std::shared_ptr<SDL_Window> getWindow(const windowId_t windowId);

    std::shared_ptr<SDL_Renderer> getWindowRenderer(const windowId_t windowId);

    windowId_t createWindow();

    windowId_t createWindow(std::shared_ptr<SDL_Window>& window);

    windowId_t createWindow(
        const std::string& windowTitle,
        const unsigned short width,
        const unsigned short height);

    windowId_t createWindow(
        std::shared_ptr<SDL_Window>& window,
        const std::string& windowTitle,
        const unsigned short width,
        const unsigned short height);


private:

    void destroyAllWindows();

    class WindowData
    {

    public:

        std::shared_ptr<SDL_Window> window;
        std::shared_ptr<SDL_Renderer> renderer;

    };

    const unsigned int DEFAULT_WINDOW_SIZE_X{800};
    const unsigned int DEFAULT_WINDOW_SIZE_Y{600};

    unsigned int mDefaultWindowSizeX;
    unsigned int mDefaultWindowSizeY;

    std::map<windowId_t, WindowData> mapWindows;

    windowId_t mNextWindowId{0};

};


#endif // SDL_RESOURCE_MANAGER_H
