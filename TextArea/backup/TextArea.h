#ifndef TEXTAREA_H
#define TEXTAREA_H



#include "Color.h"
#include "FontTexture.h"
#include "CursorType.h"


#include <SDL2/SDL.h>


#include <string>



class TextBuffer;


// class to hold data for an area of text
// the size of the text area is defined in
// pixels and as much as possible of each
// line of text is drawn. this varies depending
// on the characters contained in each line
// because they may have non-uniform widths
// with non-monospace fonts.
//
// These comments more relevant to a terminal
// emulator implementation:
// Put methods ignore out of bounds
// Get methods throw error on out of bounds
//
// contains draw method to draw to SDL_Surface
// (not currently implemented)
// needs some kind of reference to a font object
// alternatively implement draw function external
// to this class and pass in instance of this class
// and a font as arguments
//


// have a think about fixed width vs variable width fonts:
//
// this class was designed with the intention of storing the characters
// to be printed as a grid of characters
//
// therefore it was not intended to be used with variable width fonts
//
// the constructor used to take an x * y number of characters to hold in
// the grid
//
// this is a bit strange when considering printing of the characters
// to screen
//
// usually, an x * y pixel resolution would be expected instead
//
// therefore construct class with an x * y pixel resolution and convert
// this into an x * y character grid size. use the pixel resolution to draw
// the background, and use the x * y grid size to draw the characters themselves
//
// if someone uses a non-fixed width font then the grid will not be drawn
// sensibly

// A monospace TextArea is probably fundamentally different to a text grid
// for a non-uniform width font. (Unless the data is stored in a textbuffer)

// New implementation: Pass an x and y resolution in pixels. The font will
// then be used to calculate how many characters can fix on each line

class TextArea
{


    friend void fillTextGridFromTextBuffer(TextArea& textArea, TextBuffer &textBuffer);


public:

    // this constructor uses a character grid size of size_x * size_y
    // to initialize the grid. the sizePixelsX is set manually
    // by the user. this means that a nonsense value of the pixel size
    // can be supplied, the consequence is that the background colored
    // square can be a size which doesn't correspond to the area where
    // characters are printed
    TextArea(   unsigned int size_x, unsigned int size_y,
                const int sizePixelsX, const int sizePixelsY,
                std::shared_ptr<FontTexture> fontTexture)
        : mSizeX(size_x)
        , mSizeY(size_y)
        , mSizePixelsX(sizePixelsX)
        , mSizePixelsY(sizePixelsY)
        , mCharacterSizeX(0)
        , mCharacterSizeY(0)
        , mFontTexture(fontTexture)
        , mBackgroundColor(COLOR_WHITE)
        , mCursorType{cursortype_t::BLOCK}
    {
        mCharacterSizeX = fontTexture->GetWidestCharacterAdvance();
        mCharacterSizeY = fontTexture->GetFontLineSkip();

        // this constructor is weird, and does not auto-calculate
        // the character grid size, size x and y


        // create a font to use here
        /*std::cout << "loading font" << std::endl;

        SDLFontManager font_manager_liberation_mono;
        try
        {
            // create font texture
            SDLFontManager font_manager_liberation_mono_local(
                sdl_manager,
                //std::shared_ptr<SDL_Renderer>(renderer),
                renderer,
                font_filename,
                12);

            font_manager_liberation_mono =
                std::move(font_manager_liberation_mono_local);
        }
        catch(const SDLLibException &e)
        {
            std::cout << e.what() << std::endl;
        }
        catch(const std::exception &e)
        {
            std::cout << e.what() << std::endl;
        }
        */
        
    }


    // this version of the constructor sets the pixel size of the 
    // textArea object. this is the size of the background square
    // which will be drawn
    // the actual size of the grid in characters is computed from
    // the pixel size
    TextArea(
        const int sizePixelsX, const int sizePixelsY,
        std::shared_ptr<SDLFontTexture> fontTexture)
        : mSizeX(0)
        , mSizeY(0)
        , mSizePixelsX(sizePixelsX)
        , mSizePixelsY(sizePixelsY)
        , mCharacterSizeX(0)
        , mCharacterSizeY(0)
        , mFontTexture(fontTexture)
        , mBackgroundColor(COLOR_WHITE)
    {
        // set the grid size in characters
        const int fontLineSkip = fontTexture->GetFontLineSkip();
        const int widestCharacterWidth = fontTexture->GetWidestCharacterWidth();
        const int widestCharacterAdvance = fontTexture->GetWidestCharacterAdvance();

        std::cout << "Widest Character Advance: " << widestCharacterAdvance << std::endl;
        std::cout << "Widest Character Width: " << widestCharacterWidth << std::endl;

        std::cout << "W Character Width " << fontTexture->GetCharacterWidthW() << std::endl;
        std::cout << "W Character Advance " << fontTexture->GetCharacterAdvanceW() << std::endl;

        std::cout << "_ Character Width " << fontTexture->GetCharacterWidthUnderscore() << std::endl;
        std::cout << "_ Character Advance " << fontTexture->GetCharacterAdvanceUnderscore() << std::endl;

        const int characterSizeX = widestCharacterAdvance;
        const int characterSizeY = fontLineSkip;

        // store these for later calculations
        mCharacterSizeX = characterSizeX;
        mCharacterSizeY = characterSizeY;
        
        // truncate rounding (round down)
        mSizeX = sizePixelsX / characterSizeX;
        mSizeY = sizePixelsY / characterSizeY;

        // fill with blank characters (' ', space)
        fill();
    }

    void setBackgroundColor(const SDL_Color backgroundColor)
    {
        mBackgroundColor = backgroundColor;
    }


    //void Draw(std::shared_ptr<SDL_Window> sdlWindow);
    void draw(std::shared_ptr<SDL_Renderer> sdlrenderer);
        // probably can't be const

    void drawPixelSize(
        std::shared_ptr<SDL_Renderer> sdlrenderer);
    // maybe can be const since sdlrenderer is the non-const object?

    // Same as above but prints to stdout TODO
    void print(std::ostream &os);


    void setFont(std::shared_ptr<SDLFontTexture> sdlfonttexture);


    std::string getBuffer() const
    {
        return mText;
    }

    void setSizePixels(const int sizePixelsX, const int sizePixelsY);

    // sets size to zero and clears buffer
    // note this function is a bit weird because it sets the size
    // of the grid directly, so removed it
    /*
    void Clear(unsigned int size_x, unsigned int size_y)
    {
        mSizeX = size_x;
        mSizeY = size_y;
        fill();
    }
    */

    void clear();

    // silently ignores out of bounds character put request
    void put(unsigned int x, unsigned int y, char c)
    {
        if(y < mSizeY)
        {
            if(x < mSizeX)
            {
                //std::cout << "index(" << x << "," << y << ")=" << index(x, y) << std::endl;
                mText[index(x, y)] = c;

                //std::cout << "mText[" << x << "," << y << "]=" << c << std::endl;
            }
        }
        //std::cin.get();
        // TODO: remove
    }

    // wrapping/non-wrapping PutString function
    void putString(unsigned int x, unsigned int y, std::string s, bool wrap = true)
    {
        //std::cout << __func__ << " s=" << s << std::endl;
        if(wrap)
        {
            putStringWrap(x, y, s);
        }
        else
        {
            putStringNoWrap(x, y, s);
        }
    }

    // will throw an error if out of bounds
    void get(unsigned int x, unsigned int y, char &c) const
    {
        if((x >= mSizeX) || (y >= mSizeY))
        {
            std::string emsg(std::string("Error: index out of bounds in function ") + std::string(__func__));
            throw emsg;
            // TODO: do not throw instance of std string
        }
        c = mText[index(x, y)];
    }


    private:

    unsigned int index(unsigned int x, unsigned int y) const
    {
        return (y * mSizeX + x);
    }

    // will increment beyond bounds of array
    void increment(unsigned int &x, unsigned int &y) const
    {
        ++ x;
        if(x >= mSizeX)
        {
            x = 0;
            ++ y;
        }
    }

    // TODO: should really be called clear_and_fill() or just clear()
    void fill();

    // non-wrapping PutString function
    void putStringNoWrap(unsigned int x, unsigned int y, std::string s)
    {
        unsigned int posX = x;
        const unsigned int posY = y;
        for(auto it{s.cbegin()}; it != s.cend(); ++ it)
        {
            char c = *it;
            Put(posX, posY, c);
            ++ x;
            if(x >= mSizeX)
            {
                break;
            }
        }
    }

    // TODO: I don't think this function works correctly

    // wrapping PutString function
    void putStringWrap(unsigned int x, unsigned int y, std::string s)
    {
        //std::cout << __func__ << std::endl;

        unsigned int posX = x;
        unsigned int posY = y;
        for(auto it{s.cbegin()}; it != s.cend(); ++ it)
        {
            char c = *it;

            //std::cout << "Put(" << posX << "," << posY << "," << c << ")" << std::endl;

            Put(posX, posY, c);
            increment(posX, posY);
            if(y >= mSizeY)
            {
                break;
            }
        }
    }

    // TODO: implement these ?
    void drawCharAnywhere(
        std::shared_ptr<SDL_Window> sdlWindow,
        const char c,
        const int x, const int y);

    // this function cannot be easily implemented because it
    // requires knowing the advance of each character which had been
    // printed before it on the same line
    void drawChar(
        std::shared_ptr<SDL_Window> sdlWindow,
        const char c,
        const unsigned int posX,
        const unsigned int posY);


private:

    // size in number of characters
    // makes more sense for monospace fonts
    // scales with font size
    // these are being deprecated
    unsigned int mSizeX;
    unsigned int mSizeY;

    // size in pixels
    // makes sense for both monospace and
    // variable width fonts
    // does not scale with font size, independent of font size
    // these are replacing mSizeX and mSizeY
    int mSizePixelsX;
    int mSizePixelsY;

    // this class needs to store the font line skip (character size y)
    // and widest character advance (character size x)
    // this is to calculate the size x and y from the pixel size x and y
    int mCharacterSizeX;
    int mCharacterSizeY;

    // Should not need to store this?
    std::shared_ptr<SDLFontTexture> mFontTexture;

    std::string mText;

    // Should not contain any drawing code
    SDL_Color mBackgroundColor;

    // Note: could inherit and create a new class which has cursors,
    // but for now we simply insert cursor variables and logic into
    // this monolithic class
    // TODO: initialize in class constructor and add setter function for cursortype
    cursortype_t mCursorType;
    unsigned int mCursorPosX;
    unsigned int mCursorPosY;

    // in order for the cursor logic to work, we need to remember the size
    // of each line in the grid.
    // currently the grid is always populated with characters, some of which
    // are just blank spaces by default
    // should perhaps solve this by making the cursor a member of the text
    // buffer class?
    // or creating a new class which holds the textArea data, but as individual
    // lines, and this class can then render the textArea by creating a new
    // object from the class used to hold the textArea data?

};











#if 0
void TextArea::Draw(std::shared_ptr<SDL_Window> sdlWindow)
{

    // TODO: what is the best way to do this with smart pointer?
    SDL_Renderer *renderer = sdlWindow->GetRenderer();
    // TODO: is this different to my code in main() of Text-Graphics-Lib

    // rendering code copied from other project
    //SDL_Rect rsrc = map_text_chars_rect.at()


    // the following section draws a complete block of text containing
    // all rendered chars

    int text_texture_w = 0;
    int text_texture_h = 0;
    if(SDL_QueryTexture(text_texture, nullptr, nullptr,
        &text_texture_w, &text_texture_h) != 0)
    {
        std::cout << SDL_GetError() << std::endl;
    }
    else
    {
        // query was ok
        // draw whole character texture block
        int rdst_y = 0;

        SDL_Rect rsrc;
        rsrc.x = 0;
        rsrc.y = 0;
        rsrc.w = text_texture_w;
        rsrc.h = text_texture_h;
        SDL_Rect rdst(0, rdst_y, text_texture_w, text_texture_h);
        SDL_Color COLOR_GREEN(0, 255, 0);
        SDL_SetRenderDrawColor(renderer, COLOR_GREEN);
        SDL_RenderFillRect(renderer, &rdst);
        SDL_RenderCopy(renderer, text_texture, &rsrc, &rdst);

        // block ends here



        // this block draws some arbitrary strings
        // this is some old code which was inherited from
        // the Text-Graphics-Lib project before I modified 
        // the API for that project.

        // draw arbitary strings
        //rdst_y += text_line_skip;
        //rdst.y = rdst_y;
        rdst.x = 0;
        rdst.y += text_line_skip;

        std::string mytext("hello world 0123456789'''");
        int ticktock = 0;
        for(char c: mytext)
        {
            int offset_x = 0;
            int index = index_of_printable_char(c);
            if(index >= 0)
            {
                for(int count = 0;
                    count < index;
                    ++ count)
                {
                    //rsrc_x += map_text_chars_rect.at(c).w;
                    offset_x += map_text_chars_advance.at(c);
                }
                rsrc.x = map_text_chars_rect.at(c).x + offset_x;
                //rsrc.y = map_text_chars_rect.at(c).y;
            const int maxy = map_text_chars_rect.at(c).h + map_text_chars_rect.at(c).y; // TODO: remove?
                rsrc.y = text_ascent - maxy;
                //rsrc.y = text_texture_h - map_text_chars_rect.at(c).y;
                rdst.w = rsrc.w = map_text_chars_rect.at(c).w;
                rdst.h = rsrc.h = map_text_chars_rect.at(c).h;

                //rdst.y += rsrc.y;
                //const int maxy = map_text_chars_rect.at(c).h + map_text_chars_rect.at(c).y;
                rdst.y += text_ascent - maxy;
                rdst.x += map_text_chars_rect.at(c).x;

                // TODO: remove this block
                if(ticktock == 0)
                {
                    //SDL_Color COLOR_GREEN(0, 255, 0);
                    SDL_SetRenderDrawColor(renderer, COLOR_GREEN);
                    SDL_RenderFillRect(renderer, &rdst);
                }
                ticktock += 1;
                ticktock %= 2;

                SDL_RenderCopy(renderer, text_texture, &rsrc, &rdst);
                rdst.x -= map_text_chars_rect.at(c).x;
                rdst.y -= text_ascent - maxy;
                int advance = map_text_chars_advance.at(c);
                
                //rdst.y -= rsrc.y;
                
                rdst.x += advance;
                rdst.y += 0;
            }
            else
            {
                std::cout << "Error in index_of_printable_char" << std::endl;
            }

        }
    }


}
#endif
// TODO: Remove this function? This should be covered by some of my code
// in the textgraphics lib folder?












#endif // TEXTAREA_H