// Internal includes
#include "FontConfigHelper.h"

// External includes
#include <fontconfig/fontconfig.h>

// C++ includes
//#include <cstdio>
#include <iostream>
#include <string>
#include <cstring>
#include <stdexcept>


class FcNameParseException : public std::runtime_error
{

public:

    FcNameParseException(const std::string& what)
        : std::runtime_error(what)
    {
    }

};


class FcFontMatchException : public std::runtime_error
{

public:

    FcFontMatchException(const std::string& what)
        : std::runtime_error(what)
    {
    }

};



// C++ STL version
std::string fontConfigGetFontFilename(const char* fontNameSearch)
{
    // fontNameSearch: example "Mono"

    // length = 0 => error
    std::string ret;

    FcInit();
    FcConfig* config = FcInitLoadConfigAndFonts();
    FcPattern* pattern = FcNameParse((const FcChar8*)fontNameSearch);

    if(!pattern)
    {
        std::string message(
            std::string("Failed to parse name ") + fontNameSearch
        );

        throw FcNameParseException(message);
    }

    const auto success = FcConfigSubstitute(config, pattern, FcMatchPattern);
    if(success == FcFalse)
    {
        throw std::runtime_error("Error: fontConfigGetFontFilename: FcConfigSubstitute failed");
    }

    FcDefaultSubstitute(pattern);

    char* fontFile;

    FcResult result;
    FcPattern* font = FcFontMatch(config, pattern, &result);

    if(!font)
    {
        std::string message(
            std::string("Failed to match font ") + fontNameSearch
        );

        throw FcFontMatchException(message);
    }

    FcChar8* file = nullptr; 

    if(FcPatternGetString(font, FC_FILE, 0, &file) == FcResultMatch)
    {
        fontFile = (char*)file;
        const auto n = strlen(fontFile);

        if(n > 0)
        {
            ret = std::string(fontFile);
        }
        else
        {
            // error, length of string remains at zero
            throw std::runtime_error(
                "Error: FcPatternGetString");
        }
    }

    // TODO: need to call these functions before throwing
    FcPatternDestroy(font);

    FcPatternDestroy(pattern);
    FcConfigDestroy(config);
    FcFini();

    // if string length 0, then error occured, return an empty string
    return ret;
}


// C++ STL version (other verion)
// TODO: pass fontNameSearch by value?
std::string fontConfigGetFontFilename(const std::string& fontNameSearch)
{
    return fontConfigGetFontFilename(fontNameSearch.c_str());
}

