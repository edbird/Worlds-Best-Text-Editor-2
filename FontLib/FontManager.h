#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H



// Internal includes
#include "ExceptionTypes.h"
#include "SDLManager.h"
#include "FontTexture.h"
#include "FontConfigHelper.h"
#include "FontLoadProperties.h"

// TOOD: only needed for windowId_t
#include "SDLResourceManager.h"

// C++ includes
#include <memory>
#include <iostream>


// TODO: implement the swap ideom here


// this class loads instances of FontTexture into a map which
// is indexed using unique keys generated from the font filename
// and the font size
//
// TODO: font colors, styles (?)
class FontManager
{


    // Unique key which is used to reference each loaded font
    class FontKey
    {

    public:

        std::string getKeyString() const;

        // This is removed because it will likely increase coupling
        //std::string fontConfigSearchDescription; // Search description passed to font config

        std::string fontFullPath;
        int fontSize{0};
        SDL_Color color;
        windowId_t windowId{0};

    };



    // Note: The field
    // m_font_manager_init_success
    // does NOT track whether the actual FontTexture class initialized
    // successfully
    // it only tracks whether memory for the FontTextureManager class
    // was allocated successfully.
    //
    // NOTE: this has now changed, see below
    //
    // Therefore at the moment it is pointless.
    // It would make more sense if it was intialized only if the
    // FontTextureManager initialized successfully, but in this case
    // it might as well be a member variable of the FontTextureManager
    // class, which makes this class completely redundant, unless it is used
    // to supply functions to load different sizes of font or different
    // types of font.
    //
    // NOTE: it is now used to prevent passing of the sdlmanager to each
    // member function. now the variable is used to track is sdlmanager
    // initialized correctly
    //
    // NOTE: this is not how this class works, it is how the other class
    // works (sdlresourcemanager)
    //
    // Should the fc font helper functions be made members of this class?
    // Probably not, but this class should probably use them to load
    // fonts.
    // Fonts should be able to change color and size.
    // Provide a map to a font type? And a map to a font size?
    // How to handle colors?
    // NOTE: this is now managed by the fontloadproperties class
    // Perhaps the FontTextureManager class should provide a SetColor
    // function which can set the foreground color and background color
    // if one is provided (ie: not clear color).


public:

    FontManager();

    ~FontManager();

    // TODO: rule of 5


    // pass heavy argument by const reference, becase
    // we will only access the data read only
/*    int GetFontAscent(const FontLoadProperties& fontloadproperties) const
    {
    }*/

    // TODO: should the font ascent and font line skip
    // variables be part of the FontTexture class or
    // part of the FontManager class?
    // Note: They are already part of the FontManager
    // class so this is a non issue

    // TODO: probably remove these interface exposing functions




    // To figure out how to pass the arguments, need to know
    // how the function we call works
    // NOTE: done
    void loadFontTexture(
        const windowId_t windowId,
        const std::string& fontFullPath,
        const int fontSize,
        const SDL_Color color,
        std::string& validChars);

    
    // This function is the same as the LoadFontTexture function
    // but with an additional step of converting a font description
    // to a filename
    std::string loadFontTextureFromDescription(
        const windowId_t windowId,
        const std::string& fontFilenameSearchDescription,
        const int fontSize,
        const SDL_Color color,
        std::string& validChars);


    std::shared_ptr<FontTexture> getFontTexture(
        const windowId_t windowId,
        const std::string& fontFullPath,
        const int fontSize,
        const SDL_Color color) const;


    std::shared_ptr<FontTexture> getFontTexture(
        const FontKey& fontKey);


private:


    // TODO: update this function
    // create a string containing the characters to render
    /*std::string init_font_chars_string() const;*/
    // Notes: This function is used from sdlfontmanager.cpp
    //
    // this function has been removed: 2021-10-07
    // it was confusing to leave in since it is not used




private:

    const int DEFAULT_FONT_SIZE = 11;

    // map a unique key produced by FontLoadProperties class
    // to a std::shared_ptr of FontTexture
    // this object is returned to functions
    // which need to access the SDL_Texture and associated
    // data (such as font size, glyph metrics)
    std::map<std::string, std::shared_ptr<FontTexture>> mapFontTexture;

    // TODO: are the sizes and locations of each character stored?

};

#endif // FONT_MANAGER_H

