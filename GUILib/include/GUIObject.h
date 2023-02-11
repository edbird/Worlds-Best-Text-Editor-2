#ifndef GUIOBJECT_H
#define GUIOBJECT_H


#include <SDL.h>

#include <jsoncpp/json/json.h>


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

    virtual void setWindowId(const int windowId);
    virtual int getWindowId() const;

    // Template Method for drawing algorithm
    void draw() const;

    virtual void processEvent(const Json::Value &event) = 0;

    virtual int posX() const;
    virtual int posY() const;
    virtual int width() const;
    virtual int height() const;

protected:

    void autoSetOffset();
    int getOffsetX() const;
    int getOffsetY() const;

    int getDrawPosX() const;
    int getDrawPosY() const;

public:

    virtual void setPosition(const int, const int);
    virtual void setSize(const int, const int);
    virtual void setWidth(const int);
    virtual void setHeight(const int);

    void setAnchor(const Anchor_e anchor);

    void setBackgroundColor(const SDL_Color color);

protected:

    // This function defines the overall drawing algorithm. It can be overridden
    // if necessary.
    virtual void virtualDraw(std::shared_ptr<SDL_Renderer> renderer) const;

    // All GUI Objects have a defined rectangular area. This function defines
    // the algorithm for drawing the background. It is virtual, so can be
    // overridden if necessary.
    virtual void drawBackground(std::shared_ptr<SDL_Renderer> renderer) const;

    SDL_Color getBackgroundColor() const;

private:

    // Defaults to window 0 which will be the first one if created
    int mWindowId;

    int mSizeX{0};
    int mSizeY{0};

    int mPosX{0};
    int mPosY{0};

    int mOffsetX{0};
    int mOffsetY{0};

    SDL_Color mBackgroundColor{0, 0, 0, 255};

    Anchor_e mAnchor{Anchor_e::TOP_LEFT};

};


#endif // GUIOBJECT_H
