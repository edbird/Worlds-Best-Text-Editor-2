#ifndef GUIBOX_H
#define GUIBOX_H

#include <SDL.h>

#include <jsoncpp/json/json.h>

#include "GUIObject.h"

class GUIBox : public GUIObject
{

public:

    GUIBox(const int);
    GUIBox(const int, const int, const int, const int, const int);

    void processEvent(const Json::Value &event) override;

protected:

    void virtualDraw(std::shared_ptr<SDL_Renderer> renderer) const override;

    // This class does not override the background drawing behaviour of the base class
    //void drawBackground(std::shared_ptr<SDL_Renderer> renderer) const override;

};


#endif