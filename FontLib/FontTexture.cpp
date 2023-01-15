#include "FontTexture.h"

// Internal includes
////#include "Color.h"
// TODO: fix cmake problem
#include "../ColorLib/Color.h"
#include "ServiceLocator.h"

// External includes
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// C++ includes
#include <string>
#include <memory>




FontTexture::FontTexture(
    const windowId_t windowId,
    const std::shared_ptr<TTF_Font> ttfFont,
    const int fontSize,
    std::string &validChars)
        // the valid (renderable) characters are returned via this string object - TODO change to wide string?
        // TODO change to set

        : mWindowId(windowId)
        , mFontSize(fontSize)
        , mFontLineSkip(0)
        , mFontAscent(0)
{

    mWindowId = windowId;

    // Save variables obtained directly from TTF_Font queries
    mFontHeight = TTF_FontHeight(ttfFont.get());
    mFontAscent = TTF_FontAscent(ttfFont.get());
    mFontDescent = TTF_FontDescent(ttfFont.get());
    mFontLineSkip = TTF_FontLineSkip(ttfFont.get());
    // NOTE: presumably these cannot fail
    
    std::cout   << "mFontHeight=" << mFontHeight
                << " mFontAscent=" << mFontAscent
                << " mFontDescent=" << mFontDescent
                << " mFontLineSkip=" << mFontLineSkip
                << std::endl;


    /*
    set_glyph_metrics(
        //sdlfontmanager,
        ttfFont,
        font_ascent,
        fontCharsString);

    render_ascii_chars(
        //sdlfontmanager,
        ttfFont,
        sdlrenderer,
        fontCharsString);
    */

    /*
    std::string valid_fontCharsString =
        check_ascii_chars(
            ttfFont,
            fontCharsString);
    */

    validChars = getValidChars(ttfFont);

    for(const auto c : validChars)
    {
        setRenderedCharsGlyphValid.insert(c);
    }

    // DEBUG
    std::cout << "validChars=" << validChars << std::endl;

    setGlyphMetrics(ttfFont, validChars);

    renderChars(ttfFont, validChars);

    /*
    std::string fontCharsString_request = fontCharsString;

    // TODO: this function
    set_glyph_metrics_safe(
        ttfFont,
        font_ascent,
        fontCharsString_request);

    render_ascii_chars_safe(
        ttfFont,
        sdlrenderer,
        fontCharsString_request);
    */
}



FontTexture::~FontTexture()
{
    mapRenderedCharsAdvance.clear();
    mapRenderedCharsSRect.clear();
    mapRenderedCharsDRect.clear();

    mCharsTexture.reset();
}



std::string FontTexture::getValidChars(std::shared_ptr<TTF_Font> ttfFont) const
{
    std::string validChars;

    for(Uint16 c = 0; c <= 255; ++ c)
    {
        // TODO: the fonts do not seem to render properly when
        // characters outside the range of ' ' to '~' are
        // renderered. Not sure why this is the case at the
        // present time
        
        // TODO: skip "bad" characters
        ////if(c < ' ') continue; // this seems to have no effect on the rendering results
        //if(c > '~' + 50) continue; // this renders all the "normal" chars correctly, but NOT the extended ones
        ////if(c > '~') continue;

        Uint16 unsigned_c = (Uint16)c;
        // TODO: this should take a Uint16
        int isGlyphProvided = TTF_GlyphIsProvided(ttfFont.get(), (Uint16)unsigned_c);
        if(isGlyphProvided)
        {
            std::cout << "glyph provided: TRUE - index=" << isGlyphProvided << " (unsigned short)=" << (unsigned short)unsigned_c << " (unsigned char)=" << unsigned_c << std::endl;

            validChars.push_back((Uint16)c);
        }
        else
        {
            std::cout << "glyph provided: FALSE - index=" << isGlyphProvided << " (unsigned short)=" << (unsigned short)unsigned_c << " (unsigned char)=" << unsigned_c << std::endl;
        }
    }

    return validChars;
}






// note: this assumes that fontCharsString contains only valid
// renderable characters for this font
void FontTexture::setGlyphMetrics(
    const std::shared_ptr<TTF_Font> &ttfFont,
    const std::string &fontChars)
{
    
    std::cout << "mFontAscent=" << mFontAscent << std::endl;


    // technically only need the init function to have succeeded
    // to be able to call this function body
    mapRenderedCharsAdvance.clear();
    mapRenderedCharsSRect.clear();
    mapRenderedCharsDRect.clear();

    int rx_offset = 0;

    for(Uint16 c: fontChars)
    {
        int advance = 0;

        int xmin = 0;
        int xmax = 0;
        int ymin = 0;
        int ymax = 0;

        if(TTF_GlyphMetrics(ttfFont.get(), c, &xmin, &xmax, &ymin, &ymax, &advance) == -1)
        {
            printf("%s\n", TTF_GetError());

            const std::string message(
                std::string("Error in setGlyphMetrics(), failed to read glyph metrics for char ") + std::to_string(c));
                
            throw TTFLibException(message);
        }
        else
        {
            /*
            std::cout << "Next Glyph Metric: "
                        << "c=" << (int)c
                        << " xmin=" << xmin
                        << " xmax=" << xmax
                        << " ymin=" << ymin
                        << " ymax=" << ymax
                        << " advance=" << advance
                        << std::endl;
            */

            mapRenderedCharsAdvance[c] = advance;

            ////int rx = rx_offset + xmin;
            int rx = xmin;
            //int ry = ymin + mFontAscent;
            ////int ry = mFontAscent - ymax;
            int ry = mFontAscent - ymax;
            int rwidth = xmax - xmin;
            int rheight = ymax - ymin;

        /*
            chars_rect.at(c).h = maxy - miny
            chars_rect.at(c).y = miny
            dst.y = text_ascent - chars_rect.at(c).h - chars_rect.at(c).y;
            dst.y = text_ascent - maxy + miny - miny;
            dst.y = text_ascent - maxy;
        */


            SDL_Rect rs{rx + rx_offset, ry, rwidth, rheight};
            SDL_Rect rd{rx, ry, rwidth, rheight};
            // DEBUG: printing characters and sdl rect parameters
            /*
            std::cout << "c=" << c
                    << " " << xmin << " " << xmax
                    << " " << ymin << " " << ymax
                    << std::endl;
            */
            mapRenderedCharsSRect[c] = rs;
            mapRenderedCharsDRect[c] = rd;
            // TODO: currently srcrect and dstrect are the same!!!

            rx_offset += advance;
        }
    }

}



// note: this assumes that fontCharsString contains only valid
// renderable characters for this font
void FontTexture::renderChars(
    const std::shared_ptr<TTF_Font> &ttfFont,
    const std::string &fontCharsString)
{

    if(!mWindowId.has_value())
    {
        throw std::runtime_error("mWindowId does not have value");
    }
    const auto windowId = mWindowId.value();
    
    std::shared_ptr<SDL_Surface> surface(
        TTF_RenderText_Blended(
            ttfFont.get(),
            fontCharsString.c_str(),
            COLOR_TEXT_DEFAULT),
        SDL_FreeSurface);

        // TTF_RenderText_Solid
        // TTF_RenderText_Shaded
        // TTF_RenderText_Blended

    if(!surface)
    {
        throw TTFLibException("Error in renderChars(), failed to render text surface");
    }
    

    if(surface)
    {
        int width = surface->w;
        int height = surface->h;

        std::cout << "Render success: "
                    << "width=" << width << " height=" << height
                    << std::endl;

    }


    const auto sdlRenderer(ServiceLocator::getSDLResourceManager()->getWindowRenderer(windowId));

    // convert the surface into a renderable texture
    if(sdlRenderer)
    {
        mCharsTexture.reset(
            SDL_CreateTextureFromSurface(
                sdlRenderer.get(),
                surface.get()),
            SDL_DestroyTexture);

        if(!mCharsTexture)
        {
            throw SDLLibException("Error in renderChars(), SDL_CreateTextureFromSurface error");
        }
    }
    else
    {
        throw SDLLibException("Error in renderChars(), failed to get SDL renderer object");
    }

}


// NOTE: function NOT used
// TODO: TTF_GlyphIsProvided is supposed to take a 16 bit value
// not just an 8 bit char value, so there may be more renderable
// values outside of the value range 0 - 255 which we do not
// have access to because of the choice to use "char" everywhere
/*
std::string FontTexture::checkChars(
    const std::shared_ptr<TTF_Font> &ttfFont,
    const std::string &requestedFontCharsString)
{
    mapRenderedCharsGlyphValid.clear();

    std::string validFontCharsString;
    for(char c: requestedFontCharsString)
    {
        int isGlyphProvided = TTF_GlyphIsProvided(ttfFont.get(), c);

        if(isGlyphProvided)
        {
            mapRenderedCharsGlyphValid.insert(std::make_pair(c, true));

            valid_fontCharsvalidFontCharsStringString.push_back(c);
        }
        else
        {
            mapRenderedCharsGlyphValid.insert(std::make_pair(c, false));

            // do not add valud to fontCharsString
        }
    }

    return validFontCharsString;
}
*/

#if 0
// NOTE: function NOT used
// should not use both this function and the previous function, use only one
// TODO: might be able to reduce duplicate code between this function and
// the one above
void FontTexture::renderCharsSafe(
    const windowId_t windowId,
    const std::shared_ptr<TTF_Font> &ttfFont,
    const std::string &fontCharsString_request)
{

    // this is new code

    // valid values are inserted into this string
    std::string fontCharsString;
    for(Uint16 c: fontCharsString_request)
    {
        int isGlyphProvided = TTF_GlyphIsProvided(ttfFont.get(), c);

        if(isGlyphProvided)
        {
            mapRenderedCharsGlyphValid.insert(std::make_pair(c, true));

            fontCharsString.push_back(c);
        }
        else
        {
            mapRenderedCharsGlyphValid.insert(std::make_pair(c, false));

            // do not add valud to fontCharsString
        }
    }
    // TODO: put this code block into a new function call and call that function
    // from this class constructor before calling the functions
    // set_glyph_metrics and render_ascii_chars

    // this is copied and pasted from above

    // for: add characters to be rendered to string

    //if(sdlfontmanager.FontManagerInitSuccess())
    {
        // convert the sdl surface to texture 

        //SDL_Surface *text_surface = 
            // etc choose other later
            //TTF_RenderText_Solid(
            //TTF_RenderText_Shaded(

        std::shared_ptr<SDL_Surface> charsSurface(
            TTF_RenderText_Blended(
                ttfFont.get(),
                fontCharsString.c_str(),
                COLOR_TEXT_DEFAULT),
            SDL_FreeSurface);

        if(charsSurface.get() != nullptr)
        {
            //m_chars_render_success = true;
        }
        else
        {
            //m_chars_render_success = false;

            throw TTFLibException("Error in render_ascii_chars(), failed to render text surface");
        }

        // at this point can now render any character from the surface

        // convert the surface into a renderable texture
        if(sdlrenderer.get() != nullptr)
        {
            // TODO: since in the same function, do not need this flag
            //if(m_chars_render_success)
            if(charsSurface.get() != nullptr)
            {
                m_chars_texture.reset(
                    SDL_CreateTextureFromSurface(
                        sdlrenderer.get(),
                        charsSurface.get()),
                    SDL_DestroyTexture);

                if(m_chars_texture.get() != nullptr)
                {
                    m_font_render_success = true; // TODO: duplicated below
                }
                else
                {
                    throw SDLLibException("Error in render_ascii_chars(), SDL_CreateTextureFromSurface error");
                }
            }
            // this code is never reachable because the error
            // will have already been thrown
            else
            {
                throw TTFLibException("Error in render_ascii_chars(), failed to render text surface");
            }
        }
        else
        {
            throw SDLLibException("Error in render_ascii_chars(), failed to get SDL renderer object");
        }
    }

}
#endif // #if 0