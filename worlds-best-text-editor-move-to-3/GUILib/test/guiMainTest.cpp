#include "Label.h"
#include "GUIBox.h"
#include "Label.h"

#include "ColorLib.h"

#include "SDLManager.h"
#include "ServiceLocator.h"

int main()
{

    SDLManager sdlManager;
    auto sdlResourceManager(ServiceLocator::getSDLResourceManager());

    // create window and renderer - required to create font texture
    std::shared_ptr<SDL_Window> window;
    const auto windowId(sdlResourceManager->createWindow(window));
    auto renderer(sdlResourceManager->getWindowRenderer(windowId));

    auto gBox = std::make_shared<GUIBox>(windowId, 10, 10, 40, 20);
    auto gLabel = std::make_shared<Label>(windowId, 20, 100, "Hello World");

    // main infinite loop
    for(bool quit = false; quit == false; )
    {
        SDL_Event event;
        while(SDL_PollEvent(&event) != 0)
        {
            if(event.type == SDL_QUIT)
            {
                quit = true;
            }
            else
            {
                // nothing
            }
        }

        SDL_SetRenderDrawColor(renderer.get(), ColorPalette::getStatic(ColorName::WHITE));
        SDL_RenderClear(renderer.get());
        
        gBox->draw();
        gLabel->draw();

        SDL_RenderPresent(renderer.get());
    }

    return 0;
}