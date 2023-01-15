#include <ColorLib.h>


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


SDL_Color ColorPalette::get(const ColorName_e name) const
{
    return mColorMapByEnumName.at(name);
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


std::optional<SDL_Color> ColorPalette::tryGet(const ColorName_e name) const
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
    // Load enumerations
    mColorMapByEnumName.insert(std::make_pair(ColorName_e::BLACK,   {   0,      0,      0} ));
    mColorMapByEnumName.insert(std::make_pair(ColorName_e::RED,     {   255,    0,      0} ));
    mColorMapByEnumName.insert(std::make_pair(ColorName_e::GREEN,   {   0,    255,      0} ));
    mColorMapByEnumName.insert(std::make_pair(ColorName_e::BLUE,    {   0,      0,    255} ));
    mColorMapByEnumName.insert(std::make_pair(ColorName_e::YELLOW,  { 255,    255,      0} ));
    mColorMapByEnumName.insert(std::make_pair(ColorName_e::CYAN,    {   0,    255,    255} ));
    mColorMapByEnumName.insert(std::make_pair(ColorName_e::MAGENTA, { 255,      0,    255} ));
    mColorMapByEnumName.insert(std::make_pair(ColorName_e::WHITE,   { 255,    255,    255} ));

    mColorMapByEnumName.insert(std::make_pair(ColorName_e::GREY,    { 127,    127,    127} ));

    mColorMapByEnumName.insert(std::make_pair(ColorName_e::BACKGROUND, {0xCC, 0xCC, 0xCC}));
    mColorMapByEnumName.insert(std::make_pair(ColorName_e::FOREGROUND, {0, 0, 0}));
    mColorMapByEnumName.insert(std::make_pair(ColorName_e::TEXT_DEFAULT, {0, 0, 0}));
    mColorMapByEnumName.insert(std::make_pair(ColorName_e::CURRENT_LINE_BACKGROUND, {0xDD, 0xDD, 0xFF}));

    // Load string names
    for(const auto [enumName, color] : mColorMapByEnumName)
    {
        const auto stringName(mapColorEnumNameToStringName.at(enumName));
        mColorMapByStringName.insert(std::make_pair(stringName, color));
    }
}
