#ifndef FONT_TEXTURE_H
#define FONT_TEXTURE_H



//
//
//  Order of operations:
//
// 1: Load font from file
// 2: Set line skip and line ascent values (global to font)
// 2.5: initialize a vector of characters to be rendered
// 3: call render ASCII characters function
//    3a: creates SDL surface from TTF library
//    3b: creates texture using window renderer
//    3c: sets glyph metrics (unique to each character of the font)
//
// 3c should probably be moved to follow 2
//


/**
 * The purpose of this class is to create a SDL_Surface / SDL_Texture ???
 * from a TTF_Font object. The rendered surface/texture can then be used
 * for high performance rendering of text without having to create
 * entire strings of text for each word to render. (Although this might
 * be an even faster performance optimization.) Or, having to load
 * individual strings before freeing them again.
 *
 * All required rendering parameters, such as the source rectangles and
 * font height, base line skip, advance, etc, have to be maintained
 * in structures in this class, because these values will not be
 * available once the TTF_Font object is freed.
 */


// TODO: may not need to maintain state for all error conditions
// since throwing an error should force program to quit unless error is
// handled

// Internal includes
#include "ExceptionTypes.h"


// C++ includes
#include <memory>
#include <iostream>
#include <map>
#include <set>
#include <optional>


// External includes
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>



// manage the actual texture for the font
// a shared pointer to this object is passed around
// and this object should contain a function to render
// a font character to a SDL_Window object, OR
// it should contain helper functions which provide
// the required data for an external function to do
// the rendering


class FontManager;


class FontTexture
{


    // required for GetFontAscent()
    // and GetFontLineSkip() functions
    // in FontManager class
    //friend class FontManager;


    friend void write(
        std::shared_ptr<SDL_Renderer> sdlrenderer,
        std::shared_ptr<FontTexture> FontTexture,
        const Uint16 c,
        int &x, const int y,
        bool advance);

    
    friend void writeString(
        std::shared_ptr<SDL_Renderer> sdlrenderer,
        std::shared_ptr<FontTexture> FontTexture,
        const std::string &text,
        int &x, const int y,
        bool advance);


    friend void writeWithBackground(
        std::shared_ptr<SDL_Renderer> sdlrenderer,
        std::shared_ptr<FontTexture> FontTexture,
        const Uint16 c,
        int &x, const int y,
        bool advance,
        const SDL_Color &background_color);


    friend void debugDrawCharsTexture(
        std::shared_ptr<SDL_Renderer> sdlrenderer,
        std::shared_ptr<FontTexture> FontTexture,
        const int x, const int y);


public:


    FontTexture(
        const int mWindowId,
        const std::shared_ptr<TTF_Font> sdlFont,
        const int fontSize,
        std::string &validAsciiChars);

    ~FontTexture();

    /*
    std::string checkChars(
        const std::shared_ptr<TTF_Font> &ttfFont,
        const std::string &requestedFontCharsString);
    */

    // For a given font, return a string containing all the valid
    // characters
    std::string getValidChars(std::shared_ptr<TTF_Font> ttfFont) const;

    // TOOD:
    std::string initValidChars(std::shared_ptr<TTF_Font> ttfFont);

    int getFontAscent() const
    {
        return mFontAscent;
    }

    int getFontLineSkip() const
    {
        return mFontLineSkip;
    }

/*
    int getWidestCharacterAdvance() const
    {
        int maxWidth = -1;
        for(const auto& it: mapRenderedCharsAdvance)
        {
            int width = it.second;

            // DEBUG
            if(width > maxWidth)
            {
                std::cout << "new widest character found (advance): " << it.first
                          << " width=" << width << std::endl;
            }

            maxWidth = std::max(maxWidth, width);
        }
        return maxWidth;
    }

    // return the font advance, calculated from the width
    // of the "W" character, which should be the largest
    // character in the font set (probably)
    // TODO: probably change the name to match the below function
    int getCharacterWidthW() const
    {
        // can use either map_rendered_chars_srect or
        // map_rendered_chars_drect here
        //int width = map_rendered_chars_drect
        int width = mapRenderedCharsSRect.at('W').w;
        return width;
    }
    // TODO: search through the characters and return the widest one

    int getCharacterAdvanceW() const
    {
        // can use either map_rendered_chars_srect or
        // map_rendered_chars_drect here
        //int width = map_rendered_chars_drect
        int width = mapRenderedCharsAdvance.at('W');
        return width;
    }

    int getCharacterWidthUnderscore() const
    {
        // can use either map_rendered_chars_srect or
        // map_rendered_chars_drect here
        //int width = map_rendered_chars_drect
        int width = mapRenderedCharsSRect.at('_').w;
        return width;
    }

    int getCharacterAdvanceUnderscore() const
    {
        // can use either map_rendered_chars_srect or
        // map_rendered_chars_drect here
        //int width = map_rendered_chars_drect
        int width = mapRenderedCharsAdvance.at('_');
        return width;
    }


    // return the widest character
    int getWidestCharacterWidth() const
    {
        int maxWidth = -1;
        for(const auto& it: mapRenderedCharsSRect)
        {
            int width = it.second.w;

            // DEBUG
            if(width > maxWidth)
            {
                std::cout << "new widest character found: " << it.first
                          << " width=" << width << std::endl;
            }

            maxWidth = std::max(maxWidth, width);
        }
        return maxWidth;
    }
*/

private:


    void setGlyphMetrics(
        const std::shared_ptr<TTF_Font> &ttfFont,
        const std::string &fontCharsString);
    
    void renderChars(
        const std::shared_ptr<TTF_Font> &ttfFont,
        const std::string &fontCharsString);

    // a function which fills the valid characters map
    // mapRenderedCharsGlyphValid
    void renderCharsSafe(
        const std::shared_ptr<TTF_Font> &ttfFont,
        const std::string &fontCharsStringRequest);



private:

    std::optional<int> mWindowId;

    // these variables remain members of this class, as they may need
    // to be accessed by external functions for actual rendering
    // of texture to SDL_Window object and drawing of texture
    int mFontSize{0};

    int mFontLineSkip{0};
    int mFontHeight{0};
    int mFontAscent{0};
    int mFontDescent{0};

    //std::string m_chars;
    std::set<Uint16> setRenderedCharsGlyphValid;
    std::map<Uint16, int> mapRenderedCharsAdvance;
    std::map<Uint16, SDL_Rect> mapRenderedCharsSRect;
    std::map<Uint16, SDL_Rect> mapRenderedCharsDRect; // TODO: remove?
        // rect properties:
        // x, y position of coordinates of char on texture surface
        // w, h size of char on texture surface

    // Texture containing all rendered characters
    std::shared_ptr<SDL_Texture> mCharsTexture;

    // problem with trying to render characters outside of the range
    // ' ' to '~'
    // different fonts may have different ranges of renderable characters
    // and these are not known in advance, but can be determined using
    // the function call TTF_GlyphIsProvided()
    //
    // this is a bit of hack - but provides a way to determine if a
    // glyph is valid
    //
    // the process is as follows: use the function TTF_GlyphIsProvided()
    // to determine if a glyph is valid. If it is, register this in the
    // valid glyph values map, and add the character value code to the
    // string which is to be rendered. Then render that string.



};




#endif // FONT_TEXTURE_H