#include <ColorLib.h>



static const std::map<ColorName, SDL_Color> mapColorEnumNameToSDLColor {
    {ColorName::BLACK,          SDL_Color(0, 0, 0, 255)},
    {ColorName::RED,            SDL_Color(255, 0, 0, 255)},
    {ColorName::GREEN,          SDL_Color(0, 255, 0, 255)},
    {ColorName::BLUE,           SDL_Color(0, 0, 255, 255)},
    {ColorName::YELLOW,         SDL_Color(255, 255, 0, 255)},
    {ColorName::CYAN,           SDL_Color(0, 255, 255, 255)},
    {ColorName::MAGENTA,        SDL_Color(255, 0, 255, 255)},
    {ColorName::WHITE,          SDL_Color(255, 255, 255, 255)},

    {ColorName::GREY,           SDL_Color(127, 127, 127, 255)},

    {ColorName::BACKGROUND,     SDL_Color(0xCC, 0xCC, 0xCC, 0xFF)},
    {ColorName::FOREGROUND,     SDL_Color(0, 0, 0, 255)},
    {ColorName::TEXT_DEFAULT,   SDL_Color(0, 0, 0, 255)},
    {ColorName::CURRENT_LINE_BACKGROUND, SDL_Color(0xDD, 0xDD, 0xFF, 0xFF)}
};



ColorPalette::ColorPalette()
{
    // initialize default colors
    // required or program will crash when calling "Get"

    if(mInitFromFile)
    {
        initFromFile();
    }
    else
    {
        initDefault();
    }
}


SDL_Color ColorPalette::get(const std::string& name) const
{
    return mColorMapByStringName.at(name);
}


SDL_Color ColorPalette::get(const ColorName name) const
{
    return mColorMapByEnumName.at(name);
}

SDL_Color ColorPalette::getStatic(const ColorName name)
{
    return mapColorEnumNameToSDLColor.at(name);
}


std::optional<SDL_Color> ColorPalette::tryGet(const std::string& name) const
{
    if(mColorMapByStringName.count(name))
    {
        return mColorMapByStringName.at(name);
    }
    else
    {
        return std::nullopt;
    }
}


std::optional<SDL_Color> ColorPalette::tryGet(const ColorName name) const
{
    if(mColorMapByEnumName.count(name))
    {
        return mColorMapByEnumName.at(name);
    }
    else
    {
        return std::nullopt;
    }
}


void ColorPalette::initFromFile()
{
    // TODO: load these from a JSON file

    // To do this: Write an adapter class which can use any of a number of different json libraries
    // For now we will only implement the JSONCPP version
    // Then load colors with that interface here
}


void ColorPalette::initDefault()
{
    // TODO: load static data first from map mapColorEnumNameToSDLColor

    // Load enumerations
    mColorMapByEnumName.insert(std::make_pair(ColorName::BLACK,     SDL_Color(   0,      0,      0,     255) ));
    mColorMapByEnumName.insert(std::make_pair(ColorName::RED,       SDL_Color(   255,    0,      0,     255) ));
    mColorMapByEnumName.insert(std::make_pair(ColorName::GREEN,     SDL_Color(   0,    255,      0,     255) ));
    mColorMapByEnumName.insert(std::make_pair(ColorName::BLUE,      SDL_Color(   0,      0,    255,     255) ));
    mColorMapByEnumName.insert(std::make_pair(ColorName::YELLOW,    SDL_Color( 255,    255,      0,     255) ));
    mColorMapByEnumName.insert(std::make_pair(ColorName::CYAN,      SDL_Color(   0,    255,    255,     255) ));
    mColorMapByEnumName.insert(std::make_pair(ColorName::MAGENTA,   SDL_Color( 255,      0,    255,     255) ));
    mColorMapByEnumName.insert(std::make_pair(ColorName::WHITE,     SDL_Color( 255,    255,    255,     255) ));

    mColorMapByEnumName.insert(std::make_pair(ColorName::GREY,      SDL_Color( 127,    127,    127,     255) ));

    mColorMapByEnumName.insert(std::make_pair(ColorName::BACKGROUND,    SDL_Color(0xCC, 0xCC, 0xCC, 0xFF) ));
    mColorMapByEnumName.insert(std::make_pair(ColorName::FOREGROUND,    SDL_Color(0, 0, 0, 255) ));
    mColorMapByEnumName.insert(std::make_pair(ColorName::TEXT_DEFAULT,  SDL_Color(0, 0, 0, 255) ));
    mColorMapByEnumName.insert(std::make_pair(ColorName::CURRENT_LINE_BACKGROUND, SDL_Color(0xDD, 0xDD, 0xFF, 0xFF) ));

    // Load string names
    for(const auto [enumName, color] : mColorMapByEnumName)
    {
        const auto stringName(mapColorEnumNameToStringName.at(enumName));
        mColorMapByStringName.insert(std::make_pair(stringName, color));
    }
}
