#include "GUIObject.hpp"


GUIObject::GUIObject(const int windowId)
    : mWindowId(windowId)
    , mPosX{0}
    , mPosY{0}
    , mSizeX{0}
    , mSizeY{0}
    , mColor{0, 0, 0}
{
}


GUIObject::GUIObject(const int windowId, const int size_x, const int size_y)
    : mWindowId(windowId)
    , mPosX{0}
    , mPosY{0}
    , mSizeX{size_x}
    , mSizeY{size_y}
    , mColor{0, 0, 0}
{
}


GUIObject::GUIObject(const int windowId, const int pos_x, const int pos_y, const int size_x, const int size_y)
    : mWindowId(windowId)
    , mPosX{pos_x}
    , mPosY{pos_y}
    , mSizeX{size_x}
    , mSizeY{size_y}
    , mColor{0, 0, 0}
{
}


void GUIObject::drawBackground()
{
    auto renderer(ServiceLocator::getRenderer());

    if(mSizeX > 0 && mSizeY > 0)
    {
        // draw background
        SDL_Rect rect{posX() + x_off, posY() + y_off, Width(), Height()};
        SDL_SetRenderDrawColor(renderer, _background_color_r_, _background_color_g_, _background_color_b_, 0xFF);
        SDL_RenderFillRect(renderer, &rect);
    }
}


int GUIObject::posX() const
{
    return mPosX;
}


int GUIObject::posY() const
{
    return mPosY;
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


void GUIObject::SetAnchor(const Anchor_e anchor)
{
    mAnchor = anchor;
}


void GUIObject::setBackgroundColor(const SDL_Color color)
{
    mColor = color;
}

