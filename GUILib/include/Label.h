#ifndef LABEL_H
#define LABEL_H



#include <SDL.h>


#include <string>
#include <sstream>


#include "FontManager.h"

#include "GUIObject.h"

class Label : public GUIObject
{

public:

    Label(const int);
    
    Label(const int, const int, const int, const std::string& text);

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

    void processEvent(const Json::Value &event) override;

    // Creates an SDL Texture with the string text
    void setText(const std::string& text);


protected:

    // call after doing anything to object that changes the size,
    // at the moment, only changing the text contents changes the size
    // so this function must be called after the contents of _text_ is changed
    void autoSetSize();

    void virtualDraw(std::shared_ptr<SDL_Renderer> renderer) const override;

    void initializeFontManager();


private:

    std::string mText;

    std::string mFontUniqueKeyString;
    FontManager mFontManager;
    // this will indicate which font texture should be used. it will depend on the
    // font color
    // font size
    // font type (font name: eg Monospace, Times New Roman, etc.)

};


#endif
