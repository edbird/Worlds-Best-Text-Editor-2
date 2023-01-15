#ifndef GUIOBJECT_H
#define GUIOBJECT_H


#include <SDL.h>


// Indicates where position value is relative to
enum class Anchor_e
{
    TOP_LEFT,
    TOP,
    TOP_RIGHT,
    RIGHT,
    BOTTOM_RIGHT,
    BOTTOM,
    BOTTOM_LEFT,
    LEFT,
    CENTER
};


class GUIObject
{

public:

    GUIObject(const int);
    GUIObject(const int, const int, const int);
    GUIObject(const int, const int, const int, const int, const int);

    virtual void draw() const = 0;
    virtual void drawBackground() const;

    virtual int posX() const;
    virtual int posY() const;
    virtual int width() const;
    virtual int height() const;

protected:

    void autoSetOffset();
    int getOffsetX() const;
    int getOffsetY() const;

public:

    virtual void setPosition(const int, const int);
    virtual void setSize(const int, const int);
    virtual void setWidth(const int);
    virtual void setHeight(const int);

    void setAnchor(const LabelAnchor anchor);

    void setBackgroundColor(const Uint8 r, const Uint8 g, const Uint8 b);


private:

    // Defaults to window 0 which will be the first one if created
    const int mWindowId{0};

    int mSizeX{0};
    int mSizeY{0};

    int mPosX{0};
    int mPosY{0};

    int mOffsetX{0};
    int mOffsetY{0};

    SDL_Color mColor{0, 0, 0};

    mAnchor{Anchor_e::TOP_LEFT};

};


#endif // GUIOBJECT_H
