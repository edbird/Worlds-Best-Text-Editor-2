#include "GUIObject.h"

#include "SDLHelper.h"

#include "ServiceLocator.h"

GUIObject::GUIObject(const int windowId)
    : mWindowId(windowId)
    , mPosX{0}
    , mPosY{0}
    , mSizeX{0}
    , mSizeY{0}
    , mBackgroundColor{0, 0, 0}
{
}


GUIObject::GUIObject(const int windowId, const int size_x, const int size_y)
    : mWindowId(windowId)
    , mPosX{0}
    , mPosY{0}
    , mSizeX{size_x}
    , mSizeY{size_y}
    , mBackgroundColor{0, 0, 0}
{
}
// This constructor doesn't make any sense in the context of objects which have
// their size automatically determinated by their contents


GUIObject::GUIObject(const int windowId, const int pos_x, const int pos_y, const int size_x, const int size_y)
    : mWindowId(windowId)
    , mPosX{pos_x}
    , mPosY{pos_y}
    , mSizeX{size_x}
    , mSizeY{size_y}
    , mBackgroundColor{0, 0, 0}
{
}


void GUIObject::setWindowId(const int windowId)
{
    mWindowId = windowId;
}

int GUIObject::getWindowId() const
{
    return mWindowId;
}


void GUIObject::draw() const
{
    // This part of the drawing algorithm (initialization) is the same for
    // all GUI objects
    auto sdlResourceManager(ServiceLocator::getSDLResourceManager());
    auto renderer(sdlResourceManager->getWindowRenderer(getWindowId()));

    // Class specific behaviour is specified by the overridable function
    // virtualDraw. The purpose of this function is to specifiy the
    // specifics of the overall drawing algorithm.
    virtualDraw(renderer);
}

void GUIObject::virtualDraw(std::shared_ptr<SDL_Renderer> renderer) const
{
    // Base class GUIObject only has one step in the drawing algorithm:
    // Draw the background
    drawBackground(renderer);
}

void GUIObject::drawBackground(std::shared_ptr<SDL_Renderer> renderer) const
{
    // This function is virtual but it is not expected to be overriden by
    // most GUI objects
    if(width() > 0 && height() > 0)
    {
        // draw background
        SDL_Rect rect{getDrawPosX(), getDrawPosY(), width(), height()};
        SDL_SetRenderDrawColor(renderer.get(), getBackgroundColor());
        SDL_RenderFillRect(renderer.get(), &rect);
    }
}


void GUIObject::processEvent(const Json::Value &event)
{
    // Do nothing by default
}


int GUIObject::posX() const
{
    return mPosX;
}

int GUIObject::posY() const
{
    return mPosY;
}


int GUIObject::getDrawPosX() const
{
    return mPosX + mOffsetX;
}

int GUIObject::getDrawPosY() const
{
    return mPosY + mOffsetY;
}


int GUIObject::width() const
{
    return mSizeX;
}

int GUIObject::height() const
{
    return mSizeY;
}


void GUIObject::autoSetOffset()
{
    if(mAnchor == Anchor_e::TOP || mAnchor == Anchor_e::CENTER || mAnchor == Anchor_e::BOTTOM)
    {
        mOffsetX = -mSizeX / 2;
    }
    else if(mAnchor == Anchor_e::TOP_RIGHT || mAnchor == Anchor_e::RIGHT || mAnchor == Anchor_e::BOTTOM_RIGHT)
    {
        mOffsetX = -mSizeX;
    }
    else
    {
        mOffsetX = 0;
    }

    if(mAnchor == Anchor_e::LEFT || mAnchor == Anchor_e::CENTER || mAnchor == Anchor_e::RIGHT)
    {
        mOffsetY = -mSizeY / 2;
    }
    else if(mAnchor == Anchor_e::BOTTOM_LEFT || mAnchor == Anchor_e::BOTTOM || mAnchor == Anchor_e::BOTTOM_RIGHT)
    {
        mOffsetY = -mSizeY;
    }
    else
    {
        mOffsetY = 0;
    }
}


int GUIObject::getOffsetX() const
{
    return mOffsetX;
}


int GUIObject::getOffsetY() const
{
    return mOffsetY;
}


void GUIObject::setPosition(const int pos_x, const int pos_y)
{
    mPosX = pos_x;
    mPosY = pos_y;
}


void GUIObject::setSize(const int size_x, const int size_y)
{
    mSizeX = size_x;
    mSizeY = size_y;
}


void GUIObject::setWidth(const int size_x)
{
    mSizeX = size_x;
}


void GUIObject::setHeight(const int size_y)
{
    mSizeY = size_y;
}


void GUIObject::setAnchor(const Anchor_e anchor)
{
    mAnchor = anchor;
}


void GUIObject::setBackgroundColor(const SDL_Color backgroundColor)
{
    mBackgroundColor = backgroundColor;
}

SDL_Color GUIObject::getBackgroundColor() const
{
    return mBackgroundColor;
}
