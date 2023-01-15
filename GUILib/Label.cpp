#include "Label.h"


#include "Functions.hpp"
#include "GUIObject.hpp"


#include <SDL.h>
#include <SDL_ttf.h>


#include <string>
#include <sstream>





Label::Label(const FontTextureManager& ftm)
    : GUIObject()
    , _ftm_{ftm}
    , _anchor_{LabelAnchor::TOP_LEFT}
{
    set_size();
}


Label::Label(const std::string& text, const FontTextureManager& ftm)
    : GUIObject()
    , _text_{text}
    , _ftm_{ftm}
    , _anchor_{LabelAnchor::TOP_LEFT}
{
    set_size();
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


void Label::setText(const std::string& text)
{
    mText = text;

    autoSetSize();
}

// call after doing anything to object that changes the size,
// at the moment, only changing the text contents changes the size
// so this function must be called after the contents of _text_ is changed
void Label::autoSetSize()
{

    // get reference to texture chars size and texture pointers
    const std::map<const char, SDL_Texture*>& texture_chars{_ftm_.GetCharTexture()};
    const std::map<const char, SDL_Rect>& texture_chars_size{_ftm_.GetCharSize()};
    
    int c_w{texture_chars_size.at(' ').w};
    int c_h{texture_chars_size.at(' ').h};

    //GUIObject::_size_x_ = c_w * _text_.size();
    //GUIObject::_size_y_ = c_h;

    SetSize(c_w * _text_.size(), c_h);

}

void Label::Draw(SDL_Renderer *const renderer, const Uint32 timer) const
{
    drawBackground();

    autoSetOffset();

    // Get font texuure manager from service locator


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

}

