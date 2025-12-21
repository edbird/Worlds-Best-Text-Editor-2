#include "GUIBox.h"

#include "SDLHelper.h"

#include "ServiceLocator.h"

GUIBox::GUIBox(const int windowId)
    : GUIObject(windowId)
{

}

GUIBox::GUIBox(const int windowId, const int x, const int y, const int w, const int h)
    : GUIObject(windowId, x, y, w, h)
{

}

// TODO: remove and implement as strategy (?) pattern
void GUIBox::virtualDraw(std::shared_ptr<SDL_Renderer> renderer) const
{
    // The GUIBox drawing algorithm could be customized here such that
    // it differs from the algorithm used in the base class, however in
    // this case it does the same thing.
    drawBackground(renderer);
}

void GUIBox::processEvent(const Json::Value &event)
{
    // Do nothing
}
