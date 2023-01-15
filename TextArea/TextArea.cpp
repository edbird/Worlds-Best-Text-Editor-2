#include "TextArea.h"


#include "SDLHelper.h"



// TODO: this function should exactly match the behaviour
// of that in TextArea::Draw(), however at the moment it
// may not do due to the auto-advance and position variables
void TextArea::Print(std::ostream &os)
{
    int count = 0;

    for(char c: mText)
    {
        os << c;

        ++ count;
        if(count > 50) break;
    }
    os << std::endl;
}



#if 0
void TextArea::draw(std::shared_ptr<SDL_Renderer> sdlrenderer)
//std::shared_ptr<SDL_Window> sdlWindow)
{

    // TODO: what is the best way to do this with smart pointer?
    ////SDL_Renderer *renderer = sdlWindow->GetRenderer();
    // TODO: is this different to my code in main() of Text-Graphics-Lib

    // rendering code copied from other project
    //SDL_Rect rsrc = map_text_chars_rect.at()


    // the following section draws a complete block of text containing
    // all rendered chars


    int font_line_skip =
        mFontTexture->GetFontLineSkip();
        
    int font_ascent =
        mFontTexture->GetFontAscent();

    int posX = 0;
    int posY = 0;
    int t_pos_x = posX;
    //int t_pos_y = posY; // (A)

    // TODO: don't expect this to render as expected

    std::size_t index(0);
    for(auto c: mText)
    {
        //std::cout << index << " > " << c << std::endl;
        // TODO: remove

        // TODO: the data is stored in mText as a flat
        // array, and was converted to a flat array from
        // a std::vector<std::string> (not a flat array)
        // now it has been converted back to something
        // 2 dimensional. This might not be the most
        // elegant way to write this.

        // convert the character index to x, y index
        const int t_index_x = (index % mSizeX);
        const int t_index_y = (index / mSizeX);

        // TODO: remove above? (A)
        //t_pos_x = 6 * t_index_x;
        const int t_pos_y = posY + font_line_skip * t_index_y;

        // required to reset the x position when a new line starts
        if(t_index_x == 0)
        {
            t_pos_x = posX;
        }

        // exit when the number of lines in y exceeds the size
        // of the textArea
        if(t_index_y >= mSizeY) break;

        // this function takes a position as pixel coordinates
        // final argument true is the autoadvance=true argument
        write(
            sdlrenderer,
            mFontTexture,
            c, t_pos_x, t_pos_y, true);

        // TODO: implement the DrawCharacterAtPosition type of functions
        
        // increment the index of the current character in mText
        // to print
        ++ index;
    }

}



void TextArea::drawPixelSize(std::shared_ptr<SDL_Renderer> sdlrenderer)
//std::shared_ptr<SDL_Window> sdlWindow)
{

    /*
    std::cout << "Filling rect: "
              << 0 << " "
              << 0 << " "
              << mSizePixelsX << " "
              << mSizePixelsY << " " << std::endl;*/

    SDL_Rect rdst{0, 0, mSizePixelsX, mSizePixelsY};
    SDL_SetRenderDrawColor(sdlrenderer.get(), mBackgroundColor);
    SDL_RenderFillRect(sdlrenderer.get(), &rdst);
    SDL_SetRenderDrawColor(sdlrenderer.get(), COLOR_WHITE);


    // TODO: what is the best way to do this with smart pointer?
    ////SDL_Renderer *renderer = sdlWindow->GetRenderer();
    // TODO: is this different to my code in main() of Text-Graphics-Lib

    // rendering code copied from other project
    //SDL_Rect rsrc = map_text_chars_rect.at()


    // the following section draws a complete block of text containing
    // all rendered chars


    int font_line_skip =
        mFontTexture->GetFontLineSkip();
        
    int font_ascent =
        mFontTexture->GetFontAscent();

    int posX = 0;
    int posY = 0;
    int t_pos_x = posX;
    //int t_pos_y = posY; // (A)

    // TODO: don't expect this to render as expected

    std::size_t index(0);
    for(auto c: mText)
    {
        //std::cout << index << " > " << c << std::endl;
        // TODO: remove

        // TODO: the data is stored in mText as a flat
        // array, and was converted to a flat array from
        // a std::vector<std::string> (not a flat array)
        // now it has been converted back to something
        // 2 dimensional. This might not be the most
        // elegant way to write this.

        // convert the character index to x, y index
        const int t_index_x = (index % mSizeX);
        const int t_index_y = (index / mSizeX);

        // TODO: remove above? (A)
        //t_pos_x = 6 * t_index_x;
        const int t_pos_y = posY + font_line_skip * t_index_y;

        // required to reset the x position when a new line starts
        if(t_index_x == 0)
        {
            t_pos_x = posX;
        }

        // exit when the number of lines in y exceeds the size
        // of the textArea
        if(t_index_y >= mSizeY) break;

        // this function takes a position as pixel coordinates
        // final argument true is the autoadvance=true argument
        write(
            sdlrenderer,
            mFontTexture,
            c, t_pos_x, t_pos_y, true);

        // TODO: implement the DrawCharacterAtPosition type of functions
        
        // increment the index of the current character in mText
        // to print
        ++ index;
    }

}
#endif



void TextArea::setFont(std::shared_ptr<FontTexture> fontTexture)
{
    // TODO: Can't fill this in until the design decisions on how t
    // Draw the object have been made
    mFontTexture = fontTexture;

    setText(*pTextBuffer);
}


void TextArea::Clear()
{
    mText.clear();
}


#if 0
void fillTextGridFromTextBuffer(TextArea& textArea, TextBuffer &textbuffer)
{

    // TODO: need to clear the line by writing " " characters
    // if the substring is shorter than sizeX

    unsigned int sizeX{textArea.mSizeX};
    unsigned int sizeY{textArea.mSizeY};

    std::size_t scrollPos = textbuffer.mScroll;
    unsigned int uScrollPos = (unsigned int)scrollPos;
    bool wrap = textbuffer.mWrap;

    if(wrap)
    {
        std::size_t textbufferWrapLineCount(0);
        //std::size_t textbufferLineIndex(0);

        // for each line in the textbuffer object
        // lines of text are stored as: std::vector<std::string>
        //for(std::size_t ix{0}; ix < textbuffer.mLines.size(); ++ ix)
        for(const auto &line: textbuffer.mLines)
        {
            // line index is ix
            //std::size_t lineLength{textbuffer.mLines.at(ix).size()};
            std::size_t lineLength{line.size()};

            /*
            // number of lines after wrapping
            unsigned int number_of_lines =
                ((unsigned int)lineLength + sizeX - 1) / sizeX;
            */


            // new approach, iterate through line with increment of sizeX

            // when the lineLength is zero, no blank line is printed
            // should the check be i <= lineLength?
            // what happens when i + sizeX == lineLength ?
            // answer: another blank line is printed where there shouldn't be
            // one
            for(unsigned int i{0};
                (i < lineLength) || (i == 0);
                i += sizeX)
            {
                // this logic statement draws the correct output depending
                // on the scroll position
                //if((textbufferWrapLineCount <= scrollPos) &&
                if((textbufferWrapLineCount >= scrollPos) &&
                    (textbufferWrapLineCount < scrollPos + sizeY))
                {
                    // delta is the y coordinate (index) of the current line
                    // to be drawn to the screen
                    // TODO: I think this logic is wrong
                    unsigned int delta = textbufferWrapLineCount - scrollPos;
                    std::cout << "delta=" << delta << std::endl;

                    // substring to print (current graphical line, a sliced
                    // part of the current text line)
                    std::string substring =
                        textbuffer.mLines.at(ix).substr(i, sizeX);

                    std::cout << "substring=" << substring << std::endl;

                    textArea.putString(0, delta, substring); // out of bounds ok

                    ++ textbufferWrapLineCount;
                }
            }

            // efficient: quit early once textArea filled
            if(textbufferWrapLineCount >= sizeY) break;
        } // for


    }
    else
    {
        // TODO: substr() will throw error if pos > size !

        // no wrap version
        std::size_t textbufferLineCount(0);

        std::size_t scrollPosH = textbuffer.mScrollH;

        for(std::size_t ix{0}; ix < textbuffer.mLines.size(); ++ ix)
        {
            // line index is ix
            std::size_t lineLength{textbuffer.mLines.at(ix).size()};

            // no comment here

            if((textbufferLineCount <= scrollPos) &&
                (textbufferLineCount < scrollPos + sizeY))
            {
                unsigned int delta = textbufferLineCount - scrollPos;
        
                std::string substring = textbuffer.mLines.at(ix).substr(scrollPosH, sizeX);

                textArea.PutString(0, delta, substring); // out of bounds ok

                ++ textbufferLineCount;
            }

            // efficient: quit early once textArea filled
            if(textbufferLineCount >= sizeY) break;
        } // for

    } // else

}
#endif