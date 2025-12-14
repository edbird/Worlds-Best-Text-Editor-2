#ifndef COLOR_LIB_H
#define COLOR_LIB_H

// External includes
#include <SDL.h>

// C++ includes
#include <map>
#include <optional>
#include <cstdint>
#include <string>

// This isn't directly used here, but it should be available to any
// source files which use the ColorLib
#include "SDLHelper.h"

// An enumeration which can be used within code
// to select by a name instead of an integer id
enum class ColorName : uint32_t
{
    BLACK = 0,
    RED = 1,
    GREEN = 2,
    BLUE = 3,
    YELLOW = 4,
    CYAN = 5,
    MAGENTA = 6,
    WHITE = 7,

    GREY = 100,

    BACKGROUND = 1000,
    FOREGROUND = 1001,
    TEXT_DEFAULT = 1002,
    CURRENT_LINE_BACKGROUND = 1003
};


// Map enum names to string names, used for initialization
static const std::map<ColorName, std::string> mapColorEnumNameToStringName {
    {ColorName::BLACK,          "black"},
    {ColorName::RED,            "red"},
    {ColorName::GREEN,          "green"},
    {ColorName::BLUE,           "blue"},
    {ColorName::YELLOW,         "yellow"},
    {ColorName::CYAN,           "cyan"},
    {ColorName::MAGENTA,        "magenta"},
    {ColorName::WHITE,          "white"},

    {ColorName::GREY,           "grey"},

    {ColorName::BACKGROUND,     "background"},
    {ColorName::FOREGROUND,     "foreground"},
    {ColorName::TEXT_DEFAULT,   "text_default"},
    {ColorName::CURRENT_LINE_BACKGROUND, "current_line_background"}
};


// This class manages colors. It functions as a service locator
// returning color by enumeration name or string name
class ColorPalette
{

public:

    ColorPalette();

    // TODO: exceptions or better to return optional with a tryGetVersion?
    SDL_Color get(const std::string& name) const;

    SDL_Color get(const ColorName name) const;

    static SDL_Color getStatic(const ColorName name);

    std::optional<SDL_Color> tryGet(const std::string& name) const;

    std::optional<SDL_Color> tryGet(const ColorName name) const;


private:

    void initFromFile();

    void initDefault();

    // Map the color enumeration to an SDL color object
    std::map<ColorName, SDL_Color> mColorMapByEnumName;

    // Map a string name to an SDL color object
    std::map<std::string, SDL_Color> mColorMapByStringName;

    const bool mInitFromFile = false;

};

#endif // COLOR_LIB_H
