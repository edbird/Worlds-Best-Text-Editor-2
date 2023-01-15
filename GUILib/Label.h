#ifndef LABEL_H
#define LABEL_H


#include "Functions.hpp"
#include "GUIObject.hpp"


#include <SDL.h>
#include <SDL_ttf.h>


#include <string>
#include <sstream>





// label
// can be used for debugging
class Label : public GUIObject
{

public:

    // Pass in a font description object which indicates which font texture should be used
    Label();
    
    Label(const std::string& text);

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


    void setText(const std::string& text);


protected:

    // call after doing anything to object that changes the size,
    // at the moment, only changing the text contents changes the size
    // so this function must be called after the contents of _text_ is changed
    void autoSetSize();

public:

    void draw() const;



private:

    std::string mText;

    SomeFontDescriptionObject // this will indicate which font texture should be used. it will depend on the
    // font color
    // font size
    // font type (font name: eg Monospace, Times New Roman, etc.)

};


#endif // LABEL_H
