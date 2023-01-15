#ifndef FONT_CONFIG_HELPER_H
#define FONT_CONFIG_HELPER_H


// C++ includes
#include <string>


// Helper library for linux font filename search
//
// Searches for the font name in the buffer pointed at by
// `fontNameSearch`. Returns the path in `returnBuffer`.
// The buffer length is specified by `bufferSize`.
// The required buffer length is returned via the return
// value.
//
int fontConfigGetFontFilename(
    char returnBuffer[],
    const unsigned int bufferSize,
    const char* fontNameSearch);


// C++ STL version
std::string fontConfigGetFontFilename(
    const char* fontNameSearch);


// TODO: now I need a C++ version with a std::string argument
// how should I pass this? by value?
std::string fontConfigGetFontFilename(
    const std::string &fontNameSearch);


    

#endif // FONT_CONFIG_HELPER_H