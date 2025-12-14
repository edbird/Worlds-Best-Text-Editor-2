#include "FontManager.h"

// Internal includes
#include "ServiceLocator.h"
#include "SDLManager.h"


std::string FontManager::FontKey::getKeyString() const
{
    // Combine all properties into one (hopefully unique) string

    std::string key(fontFullPath);

    // remove special characters
    for(auto it{key.begin()}; it != key.end(); ++ it)
    {
        if(*it == '/')
        {
            *it = '_';
        }
    }

    key += std::string("_FontSize_") + std::to_string(fontSize);
    key +=  std::string("_rgb")
        +   std::string("_") + std::to_string(color.r)
        +   std::string("_") + std::to_string(color.g)
        +   std::string("_") + std::to_string(color.b)
        +   std::string("_window_id_") + std::to_string(windowId);

    return key;

}


FontManager::FontManager()
{

}


FontManager::~FontManager()
{   
}


// TODO: the validChars should probably be detected by loading the font from file
// not yet sure how to do this however
void FontManager::loadFontTexture(
    const windowId_t windowId,
    const std::string& fontFullPath,
    const int fontSize,
    const SDL_Color color,
    std::string &fontUniqueKeyString,
    std::string& validChars)
{
    /*
    const auto sdlManager(ServiceLocator::getSDLManager());
    if(sdlManager::getInstance().TTFInitSuccess())
    {
        // Do nothing
    }
    else
    {
        throw TTFLibException("Error in FontManager(), TTF library previously failed to initialize");
    }*/

    const FontKey fontKey
    {
        .fontFullPath = fontFullPath,
        .fontSize = fontSize,
        .color = color,
        .windowId = windowId            // Since FontTextures create SDL_Texture objects
                                        // which are unique to each window, the window id
                                        // must be stored to access a particular
                                        // FontTexture
    };

    const auto uniqueKey(fontKey.getKeyString());

    if(mapFontTexture.find(uniqueKey) == mapFontTexture.cend())
    {
        if(SDLManager::initSuccess())
        {
            // This object is used to initialize the font texture,
            // it can then be discarded. Used by functions:
            // - renderAsciiChars
            // - setGlyphMetrics

            std::shared_ptr<TTF_Font> pFont; // TODO: use make_shared
            pFont.reset(TTF_OpenFont(fontFullPath.c_str(), fontSize),
                        TTF_CloseFont);

            // TODO: fontManager doesn't need access to the sdlRenderer...
            // we can obtain this from within the FontTexture class itself

            if(pFont)
            {
                // Disable kerning
                TTF_SetFontKerning(pFont.get(), 0);
                // Disable font hinting
                TTF_SetFontHinting(pFont.get(), TTF_HINTING_NONE);

                // TODO: const here?
                /*const std::string font_chars_string =
                    init_font_chars_string();*/
                // TODO: this function has now been removed because (I thought)
                // it was not used anywhere... turns out it WAS used here, but
                // it SHOULD be moved inside of FontTexture regardless

                std::shared_ptr<FontTexture> fontTexture;

                // create the FontTextureManager
                // TODO: this needs to return the renderable characters
                // for now do this using std::string, but some fonts
                // support unicode / 16bit codes, therefore a std::string
                // is not sufficient (but perhaps a wide string is)
                /*std::string validChars;*/
                fontTexture.reset(
                    new FontTexture(
                        windowId,
                        pFont,
                        fontSize,
                        validChars));
                // this will (may?) fail here if font_chars_string
                // contains invalid characters which cannot be rendered
                // ... the point of failure was confusing because of the
                // throwing and then catching of errors

                if(fontTexture.get())
                {
                    // save the fontTexture in the map
                    mapFontTexture.insert(std::make_pair(uniqueKey, fontTexture));
                }
                else
                {
                    throw TTFLibException("Error: Failed to create sdlfonttexture");
                }
            }
            else
            {
                std::cout << "Error: Could not load font from file: "
                        << fontFullPath << std::endl;
                std::cout << TTF_GetError() << std::endl;

                throw TTFLibException(
                    "Error in font_init(), failed to load font from file");
            }
        }
        else
        {
            throw TTFLibException(
                "Error in loadFontTexture(): Class FontManager failed to initialize. Probably the TTF library failed to initialize");
        }

        fontUniqueKeyString = uniqueKey;
    }
    else
    {
        std::string serror =
            std::string("Error: map_font_texture already contains unique key: ") + uniqueKey;

        throw TTFLibException(serror);
    }
}



std::string FontManager::loadFontTextureFromDescription(
    const windowId_t windowId,
    const std::string& fontFilenameSearchDescription,
    const int fontSize,
    const SDL_Color color,
    std::string &fontUniqueKeyString,
    std::string &validChars)
{
    std::string fontFullPath =
        fontConfigGetFontFilename(fontFilenameSearchDescription);

    loadFontTexture(windowId, fontFullPath, fontSize, color, fontUniqueKeyString, validChars);
    std::cout << "finished LoadFromTexture function, return: " << fontFullPath << std::endl;

    return fontFullPath;
}



std::shared_ptr<FontTexture> FontManager::getFontTexture(
    const windowId_t windowId,
    const std::string& fontFullPath,
    const int fontSize,
    const SDL_Color color) const
{
    FontKey fontKey {
        .fontFullPath = fontFullPath,
        .fontSize = fontSize,
        .color = color,
        .windowId = windowId
    };

    const std::string uniqueKey(fontKey.getKeyString());

    return getFontTexture(uniqueKey);
}


std::shared_ptr<FontTexture> FontManager::getFontTexture(
    const FontKey& fontKey) const
{
    const std::string uniqueKey(fontKey.getKeyString());

    return getFontTexture(uniqueKey);
}


std::shared_ptr<FontTexture> FontManager::getFontTexture(
    const std::string &fontUniqueKeyString) const
{
    const std::string& uniqueKey(fontUniqueKeyString);

    if(mapFontTexture.find(uniqueKey) == mapFontTexture.cend())
    {
        std::string message(
            std::string("Error: mapFontTexture does not contain unique key: ") + uniqueKey
        );

        throw TTFLibException(message);
    }
    else
    {
        return mapFontTexture.at(uniqueKey);
    }
}


// TODO: in order for this function to check if each character
// can be rendered by the font, it needs to call the function
// TTF_GlyphIsProvided, which requires a TTF_Font object
// therefore this function is not universal and needs to be
// placed in the sdlfonttexture class
//
// Note: This function is NOT used in this class. It WAS used
// in the removed function: FontManager::font_manager_init
// but this function no longer exists
/*
std::string
FontManager::init_font_chars_string() const
{
    //m_font_chars_string_init_success = true;

    // add characters to be rendered to string
    std::string font_chars_string;
    //for(char c = ' '; c <= '~'; ++ c)

    // try to initialize all available glyphs
    //int is_glyph_provided = TTF_GlyphIsProvided(font, code);
    const int c_start = 0; // ' '
    const int c_end = (int)255; // '~'
    for(int c = c_start; c <= c_end; ++ c)
    {
        unsigned char unsigned_c = (unsigned char)c;
        font_chars_string.push_back(unsigned_c);
    }

    return font_chars_string;
}
*/


// TODO: check the sdlfonttexture.cpp file and ensure all the code here
// has been migrated there
// currently a check for the correct initialization of the sdlmanager
// (sdl libs) is missing
/*
void FontManager::font_manager_init(
    const SDLManager &sdlmanager,
    std::shared_ptr<SDL_Renderer> sdlrenderer,
    const std::string &font_path,
    const int fontSize)
{

    // TODO: these shared pointers were members of the
    // FontManager class but they do not need to be saved
    // for future use, therefore move them to be local variables
    // such that storage will be freed after use

    std::shared_ptr<TTF_Font> m_font;
    // TODO: m_chars_surface is not used here, remove?
    //std::unique_ptr<SDL_Surface> m_chars_surface;



        // TODO: these checks related to the initialization of the
        // SDL libraries
        // so this class needs to be managed from within
        // the SDLManager class

        // Font only needs to check ttf_init
    if( sdlmanager.TTFInitSuccess() //&&
        //m_window_init_success &&
        //m_renderer_init_success
        )
    {
        //TTF_Font *font = nullptr;
        //const int fontSize = DEFAULT_FONT_SIZE; // 12
        m_font.reset(TTF_OpenFont(font_path.c_str(), fontSize),
            TTF_CloseFont);

        if(m_font.get() != nullptr)
        {

            // NOTE: presumably these cannot fail
            const int font_line_skip = TTF_FontLineSkip(m_font.get());
            const int font_ascent = TTF_FontAscent(m_font.get());
            //m_font_properties_success = true; // TODO

            //if(m_font_properties_success)
                // no point continuing if the previous step didn't work
            {
                // TODO: const here?
                const std::string font_chars_string = init_font_chars_string();


                // create the FontTextureManager
                m_sdlfonttexturemanager.reset(
                    new FontTexture(
                        sdlrenderer,
                        m_font,
                        fontSize,
                        font_line_skip,
                        font_ascent,
                        font_chars_string));

                // NOTE: managed internally
                //m_sdlfonttexturemanager->set_glyph_metrics(font_chars_string);
                //m_sdlfonttexturemanager->render_ascii_chars(font_chars_string);

                if(m_sdlfonttexturemanager.get() != nullptr)
                {
                    m_font_manager_init_success = true;
                    // TODO: maybe can remove this variable, does nothing?

                    // NOTE: this variable tracks whether the memory was 
                    // allocated and not whether the actual FontTexture
                    // was able to initialize successfully
                }
                else
                {
                    // TODO: what type of error here?
                    throw SDLLibException("error in FontManager::font_manager_init, failed to allocate instance of FontTexture");
                }
            }
        }
        else
        {
            std::cout << "Error: Could not load font from file: "
                      << font_path << std::endl;
            std::cout << TTF_GetError() << std::endl;

            throw TTFLibException(
                "Error in font_init(), failed to load font from file");
        }
    }
    else
    {
        throw TTFLibException(
            "Error in font_init(), TTF library previously failed to initialize");
    }
}
*/


