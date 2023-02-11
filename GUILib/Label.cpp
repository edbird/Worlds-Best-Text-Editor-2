    #include "Label.h"

#include "GUIObject.h"


#include <SDL.h>
#include <SDL_ttf.h>


#include <string>
#include <sstream>

#include "ColorLib.h"



Label::Label(const int windowId)
    : GUIObject(windowId)
{
    initializeFontManager();

    // TODO
    //set_size();
}


Label::Label(const int windowId, const int posX, const int posY, const std::string& text)
    : GUIObject(windowId, posX, posY, 0, 0)
    , mText{text}
{
    initializeFontManager();

    setText(text);

    // TODO
    //set_size();
}

void Label::initializeFontManager()
{
    std::string validChars;
    const std::string fontName("Liberation Mono");
    const int fontSize = 12;

    // This data is used to reference the font to load
    // the font texture required for drawing
    std::string fontUniqueKeyString;

    mFontManager.loadFontTextureFromDescription(
        getWindowId(),
        fontName,
        fontSize,
        ColorPalette::getStatic(ColorName::BLACK),
        fontUniqueKeyString,
        validChars
    );

    mFontUniqueKeyString = fontUniqueKeyString;
}

/*
int Width() const
{
    // get reference to texture chars size and texture pointers
    const std::map<const char, SDL_Texture*>& texture_chars{_ftm_.GetCharTexture()};
    const std::map<const char, SDL_Rect>& texture_chars_size{_ftm_.GetCharSize()};
    
    int c_w{texture_chars_size.at(' ').w};
    //int c_h{texture_chars_size.at(' ').h};

    return c_w * _text_.size();
}
*/


// TODO: base class with width and height for drawable objects
/*
int Height() const
{
    // get reference to texture chars size and texture pointers
    const std::map<const char, SDL_Texture*>& texture_chars{_ftm_.GetCharTexture()};
    const std::map<const char, SDL_Rect>& texture_chars_size{_ftm_.GetCharSize()};
    
    //int c_w{texture_chars_size.at(' ').w};
    int c_h{texture_chars_size.at(' ').h};

    return c_h;
}
*/


void Label::processEvent(const Json::Value &event)
{
    // Do nothing
}


void Label::setText(const std::string& text)
{
    // TODO: check that chars in text are valid using
    // validChars which is initialized in the function
    // initializeFontManager
    
    mText = text;

    // Initialize a texture using the specified text
    // TODO
    /*std::shared_ptr<FontTexture> fontTexture = fontManager.getFontTexture(
        windowId, fontFilenameLiberationMono, 12, ColorPalette::getStatic(ColorName::BLACK));*/

    autoSetSize();
}

// call after doing anything to object that changes the size,
// at the moment, only changing the text contents changes the size
// so this function must be called after the contents of _text_ is changed
void Label::autoSetSize()
{

    // get reference to texture chars size and texture pointers
    ////const std::map<const char, SDL_Texture*>& texture_chars{_ftm_.GetCharTexture()};
    ////const std::map<const char, SDL_Rect>& texture_chars_size{_ftm_.GetCharSize()};
    
    ////int c_w{texture_chars_size.at(' ').w};
    ////int c_h{texture_chars_size.at(' ').h};

    //GUIObject::_size_x_ = c_w * _text_.size();
    //GUIObject::_size_y_ = c_h;

    ////SetSize(c_w * _text_.size(), c_h);

}

void Label::virtualDraw(std::shared_ptr<SDL_Renderer> renderer) const
{
    drawBackground(renderer);

    // TODO
    //autoSetOffset();

    // Get font texuure manager from service locator


    const auto fontTexture = mFontManager.getFontTexture(mFontUniqueKeyString);

    int x = getDrawPosX();
    const int y = getDrawPosY();
    writeString(renderer, fontTexture, mText, x, y, true);

#if 0
    // get reference to texture chars size and texture pointers
    const std::map<const char, SDL_Texture*>& texture_chars{_ftm_.GetCharTexture()};
    const std::map<const char, SDL_Rect>& texture_chars_size{_ftm_.GetCharSize()};
    
    int c_w{texture_chars_size.at(' ').w};
    int c_h{texture_chars_size.at(' ').h};


    // draw text
    // TODO: use the actual text drawing function for this purpose    
    SDL_Rect dst_rect{PosX() + x_off, PosY() + y_off, c_w, c_h};
    SDL_Rect src_rect{0, 0, c_w, c_h};

    for(std::size_t char_ix{0}; char_ix < _text_.size(); ++ char_ix)
    {
        // character to print
        const char ch{_text_[char_ix]};
    
        // set the texture pointer
        SDL_Texture *texture{texture_chars.at(ch)};
        print_char_texture(renderer, texture, src_rect, dst_rect);
        
        // advance dst
        // TODO: can print off screen
        dst_rect.x += dst_rect.w;
    }
#endif

}

