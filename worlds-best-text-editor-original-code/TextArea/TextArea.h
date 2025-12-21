#ifndef TEXTAREA_H
#define TEXTAREA_H



#include "Color.h"
#include "FontTexture.h"
#include "CursorType.h"


#include <SDL2/SDL.h>


#include <string>


// TODO:
//
// I don't think it makes much sense to separate the Drawable parts of this class
// because the fontTexture is inherently related to the drawing of the object
// and it is also needed to figure out how wide each character is - in order to
// determine the length of each line of text.
//
// Separate out the logic for scrolling and drawing of the cursor.
//
//


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

    TextArea(   const int sizePixelsX, const int sizePixelsY,
                std::shared_ptr<FontTexture> fontTexture)
        : pTextBuffer(nullptr)
        , mSizePixelsX(sizePixelsX)
        , mSizePixelsY(sizePixelsY)
        , mPosX(0)
        , mPosY(0)
        , mFontTexture(fontTexture)
        , mBackgroundColor(COLOR_WHITE)
    {
    }

    // Initialize the text using a TextBuffer object
    TextArea(   const int sizePixelsX, const int sizePixelsY,
                std::shared_ptr<FontTexture> fontTexture,
                const TextBuffer &textBuffer)
        : pTextBuffer(&textBuffer)
        , mSizePixelsX(sizePixelsX)
        , mSizePixelsY(sizePixelsY)
        , mPosX(0)
        , mPosY(0)
        , mFontTexture(fontTexture)
        , mBackgroundColor(COLOR_WHITE)
    {
        // set the grid size in characters
        const int fontLineSkip = fontTexture->GetFontLineSkip();
        const int widestCharacterWidth = fontTexture->GetWidestCharacterWidth();
        const int widestCharacterAdvance = fontTexture->GetWidestCharacterAdvance();

        const int characterSizeX = widestCharacterAdvance;
        const int characterSizeY = fontLineSkip;

        setText(textBuffer);
    }

    void setBackgroundColor(const SDL_Color &backgroundColor)
    {
        mBackgroundColor = backgroundColor;
    }

    void setPosition(const int posX, const int posY)
    {
        mPosX = posX;
        mPosY = posY;
    }


    //void Draw(std::shared_ptr<SDL_Window> sdlWindow);
    void draw(std::shared_ptr<SDL_Renderer> sdlrenderer);
        // probably can't be const

    void drawPixelSize(
        std::shared_ptr<SDL_Renderer> sdlrenderer);
    // maybe can be const since sdlrenderer is the non-const object?

    // Same as above but prints to stdout TODO
    void print(std::ostream &os);

    void setFont(std::shared_ptr<FontTexture> fontTexture);

    void setSizePixels(const int sizePixelsX, const int sizePixelsY);

    void clear();


private:

    // Set the mText object, which contains lines of text to be printed
    // Initialize the vector of strings from a textBuffer object
    void setText(const TextBuffer& textBuffer)
    {
        pTextBuffer = &textBuffer;

        mText.clear();

        // For each line of text
        for(const auto &line: textBuffer) // should it be for each line in textbuffer or textbuffer.something
        {
            // Line width in pixels, depending on which characters are drawn
            int lineWidth = 0;

            // Index of starting point of text and length of text
            // Used to create substrings to insert into the vector of strings
            std::size_t indexStart = 0;
            std::size_t indexLength = 0;

            newLine = false;
            for(const auto character: line)
            {
                if(character == '\n' || character == '\r')
                {
                    if(newLine == false)
                    {
                        mText.push_back(line.substr(indexStart, indexLength));

                        lineWidth = 0;
                        indexStart += indexLength;
                        indexLength = 0;
                    }
                    newLine = true;
                }
                else
                {
                    const auto advance = mFontTexture->mapRenderedCharsAdvance.at(character);

                    if(lineWidth + advance < mSizePixelsX)
                    {
                        lineWidth += advance;
                        ++ indexLength;
                    }
                    else
                    {
                        // Reached limit of line
                        mText.push_back(line.substr(indexStart, indexLength));

                        lineWidth = 0;
                        indexStart += indexLength;
                        indexLength = 0;
                    }
                }
            }
            if(indexLength > 0)
            {
                mText.push_back(line.substr(indexStart, indexLength));
            }
        }
    }


    // TODO move windowId into the class constructor
    // It should NOT change between calls to draw()
    void draw(const windowId_t windowId)
    {
        //const auto resourceManager = ServiceLocator::getSDLResourceManager();
        //auto pWindow = resourceManager.getWindow(windowId);

        auto renderer(ServiceLocator::getRenderer(windowId));

        SDL_Rect rectBackground{mPosX, mPosY, mSizePixelsX, mSizePixelsY};
        SDL_SetRenderDrawColor(renderer, mBackgroundColor);
        SDL_RenderFillRect(renderer, &rectBackground);

        for(const auto line: mText)
        {
            writeString(renderer, line);
        }
    }


private:

    const TextBuffer *pTextBuffer;

    // Operations which cause mText to be re-calculated
    //
    // - changing the textbuffer
    // - changing the font
    // - changing the area of this widget (size pixels x, y)

    // size in pixels
    // makes sense for both monospace and
    // variable width fonts
    // does not scale with font size, independent of font size
    // these are replacing mSizeX and mSizeY
    int mSizePixelsX = 0;
    int mSizePixelsY = 0;

    std::shared_ptr<FontTexture> mFontTexture;

    // The text to be written as part of the drawing process
    // organized into individual lines, each line is drawn
    // by calling one of the writeString functions
    std::vector<std::string> mText;

    int mPosX = 0;
    int mPosY = 0;

    // Should not contain any drawing code
    SDL_Color mBackgroundColor;

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