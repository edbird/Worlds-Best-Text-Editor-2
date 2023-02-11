#include "WriteFunctions.h"

// Internal includes
#include "SDLHelper.h"
#include "FontTexture.h"

// External includes
#include <SDL2/SDL.h>


void debugDrawCharsTexture(
    std::shared_ptr<SDL_Renderer> sdlRenderer,
    std::shared_ptr<FontTexture> fontTexture,
    const int x, const int y)
{
    // Draws the entire rendered texture
    
    int width = 0;
    int height = 0;

    // TODO: check return value
    int return_value =
        SDL_QueryTexture(fontTexture->mCharsTexture.get(),
            nullptr, nullptr, &width, &height);

    SDL_Rect rsrc = {0, 0, width, height};
    SDL_Rect rdst = {x, y, width, height};

    //rdst.x += x;
    //rdst.y += y;

    SDL_RenderCopy(
        sdlRenderer.get(),
        fontTexture->mCharsTexture.get(),
        &rsrc, &rdst);
}


// TODO: these are the autoadvance versions of the functions
// TODO: write some non-autoadvancing functions


// documentation notes on argument `advance`
//
// all printable characters have a width, which is stored in the object
// fontTexture->mapRenderedCharsAdvance
// 

void write(
    std::shared_ptr<SDL_Renderer> sdlRenderer,
    std::shared_ptr<FontTexture> fontTexture,
    const Uint16 c,
    int &x, const int y,
    const bool advance)
{

    // Do nothing if the font does not contain the character
    if(!fontTexture->setRenderedCharsGlyphValid.contains(c))
    {
        return;
    }

    int x_copy = x;
    int y_copy = y;

    SDL_Rect rsrc = fontTexture->mapRenderedCharsSRect.at(c);
    SDL_Rect rdst = fontTexture->mapRenderedCharsDRect.at(c);
    //SDL_Rect rdst = {x, y, rsrc.w, rsrc.h};

    //std::cout << "x=" << x << std::endl;

    rdst.x += x_copy;
    rdst.y += y_copy;

    SDL_RenderCopy(
        sdlRenderer.get(),
        fontTexture->mCharsTexture.get(),
        &rsrc, &rdst);

    const int x_advance = fontTexture->mapRenderedCharsAdvance.at(c);
    x_copy += x_advance;


    // TODO: how should this map work?
    // what is the most useful data to store?
    // SDL_RenderCopy uses a src_rect
    // so should just store the src_rect
    // the x,y values are the starting position of the char
    // in the source texture
    // and width,height are the width and height of the area
    // to copy with the rendercopy function

    if(advance)
    {
        x = x_copy;
    }

}


void writeWithBackground(
    std::shared_ptr<SDL_Renderer> sdlRenderer,
    std::shared_ptr<FontTexture> fontTexture,
    const Uint16 c,
    int &x, const int y,
    const bool advance,
    const SDL_Color &backgroundColor)
{

    // Do nothing if the font does not contain the character
    if(!fontTexture->setRenderedCharsGlyphValid.contains(c))
    {
        return;
    }

    int x_copy = x;
    int y_copy = y;

    SDL_Rect rsrc = fontTexture->mapRenderedCharsSRect.at(c);
    SDL_Rect rdst = fontTexture->mapRenderedCharsDRect.at(c);
    //SDL_Rect rdst = {x, y, rsrc.w, rsrc.h};

    rdst.x += x_copy;
    rdst.y += y_copy;

    SDL_SetRenderDrawColor(sdlRenderer.get(), backgroundColor);
    SDL_RenderFillRect(sdlRenderer.get(), &rdst);
    SDL_RenderCopy(
        sdlRenderer.get(),
        fontTexture->mCharsTexture.get(),
        &rsrc, &rdst);

    const int x_advance = fontTexture->mapRenderedCharsAdvance.at(c);
    x_copy += x_advance;

    if(advance)
    {
        x = x_copy;
    }
}


// TODO: versions of these functions which compute the drawable area

void writeString(
    std::shared_ptr<SDL_Renderer> sdlRenderer,
    std::shared_ptr<FontTexture> fontTexture,
    const std::string &text,
    int &x, const int y,
    const bool advance)
{

    int x_copy = x;
    int y_copy = y;

    //int local_x = x;
    for(auto c: text)
    {
        // if advance is true, the changed values of x propagate
        // back through this function and the called function below `write`
        // if advance is false, the changed values of x do not propagate
        // back through this function, however the character printing
        // position is advanced internally to the `write` function (below)
        // otherwise all the characters of the string are printed on
        // top of each other, which makes no sense for a string printing
        // function
        write(sdlRenderer, fontTexture, c, x_copy, y_copy, true);
    }

    if(advance)
    {
        x = x_copy;
    }
}


void writeStringWithBackground(
    std::shared_ptr<SDL_Renderer> sdlRenderer,
    std::shared_ptr<FontTexture> fontTexture,
    const std::string &text,
    int &x, const int y,
    const bool advance,
    const SDL_Color &backgroundColor)
{

    int x_copy = x;
    int y_copy = y;

    for(auto c: text)
    {
        writeWithBackground(sdlRenderer, fontTexture, c, x_copy, y_copy, true, backgroundColor);
    }

    if(advance)
    {
        x = x_copy;
    }
}


void writeStringWithTickTockBackground(
    std::shared_ptr<SDL_Renderer> sdlRenderer,
    std::shared_ptr<FontTexture> fontTexture,
    const std::string &text,
    int &x, const int y,
    const bool advance,
    const SDL_Color &backgroundColor1,
    const SDL_Color &backgroundColor2)
{
    int tickTock = 0;

    int x_copy = x;
    int y_copy = y;

    const SDL_Color *backgroundColor = &backgroundColor1;

    for(auto c: text)
    {
        writeWithBackground(sdlRenderer, fontTexture, c, x_copy, y_copy, true, *backgroundColor);

        if(tickTock)
        {
            backgroundColor = &backgroundColor1;
        }
        else
        {
            backgroundColor = &backgroundColor2;
        }

        ++ tickTock;
        tickTock %= 2;
    }

    if(advance)
    {
        x = x_copy;
    }
}



// NOTE: this is a bit weird because in order to know the size of the
// rectangle (custom symbol) which should be drawn, the font texture
// is required to query the size of the characters in the font
//
// TODO: would it be better to draw actual font characters for the
// caret symbol? Do the fonts (in general) support some special
// characters for this? If not, I could potentially draw custom
// font symbols and include them in the fontTexture
// This would seem to be the most sensible approach as the
// font texture will store all data for drawing any of the required
// symbols.
//
// Problems: font (at least liberation mono) does not appear to have
// a caret symbol included, and trying to render beyond the standard
// (' ' to '~') ascii character range caused the rendering to break
//
// The difficulty arrises when mapping ascii values to these font
// symbols. Would need an ascii value for a block character. Or a
// vertical line, which doesn't exist (?) in the ascii table.
// May have to extend the data type from "char" to something else.
// At the moment I have used a TextGridElement class to hold the
// expanded data type.
#if 0
void writeCustomSymbol(
    std::shared_ptr<SDL_Renderer> sdlRenderer,
    std::shared_ptr<FontTexture> fontTexture,
    //const char c,
    int &x, const int y,
    const bool advance)
{
    // this function was never completed?

    int x_copy = x;
    int y_copy = y;

    //SDL_Rect rsrc = fontTexture->mapRenderedCharsSRect.at(c);
    SDL_Rect rdst = fontTexture->mapRenderedCharsDRect.at(c);
    //SDL_Rect rdst = {x, y, rsrc.w, rsrc.h};

    //std::cout << "x=" << x << std::endl;

    rdst.x += x_copy;
    rdst.y += y_copy;

    SDL_RenderCopy(
        sdlRenderer.get(),
        fontTexture->mCharsTexture.get(),
        &rsrc, &rdst);

    const int x_advance =
        fontTexture->mapRenderedCharsAdvance.at(c);
    x_copy += x_advance;

    if(advance == true)
    {
        x = x_copy;
    }

}
#endif