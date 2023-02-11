
/*

    TODO:

    Other libraries to write:
        - config
        - program arguments parser
        - SDL font / resources manager
        - gui objects for text editor?
        - text editor program

*/



#include <SDL2/SDL.h>
//#include <SDL2/SDL_ttf.h>



#include <iostream>

#include <map>

#include <vector>



#include "SDLManager.h"
#include "SDLResourceManager.h"
#include "FontManager.h"
#include "ServiceLocator.h"





#include "SDLHelper.h"


#include "ColorLib.h"
#include "WriteFunctions.h"


// TODO: general comment: use service locators to locate objects rather
// than referencing them directly
// use shared_ptr rather than references





#include "FontConfigHelper.h"



#define FONT_TIMES_NEW_ROMAN 1
int main(int argc, char* argv[])
{


    //Config config;
    //config.Load("config.txt");


    SDLManager sdlManager;

    auto sdlResourceManager(ServiceLocator::getSDLResourceManager());


    // "/usr/share/fonts/truetype/ttf-bitstream-vera/VeraMono.ttf"
    /*const int FONT_FILENAME_BUFFER_SIZE = 4096;
    char font_filename_buffer[FONT_FILENAME_BUFFER_SIZE];
    fontConfigGetFontFilename(font_filename_buffer,
        FONT_FILENAME_BUFFER_SIZE, "Liberation Mono");

    std::string fontFilename(font_filename_buffer);*/
/*
    std::string fontFilename;
    try
    {
        fontFilename = fontConfigGetFontFilename("Liberation Mono");
    }
    catch(std::exception &e)
    {
        std::cout << "e.what() = " << e.what() << std::endl;
        throw;
    }
    std::cout << "Matched font filename: " << fontFilename << std::endl;
*/


    // testing the case of a font that does not exist
    std::cout << "does this crash? ..." << std::endl;
    try
    {
        std::string fontFilename = fontConfigGetFontFilename("Nothing");
        std::cout << fontFilename << std::endl;
    }
    catch(std::exception &e)
    {
        // TODO: test this code as exe
    }

    std::cout << "Did not crash where expected" << std::endl;

    // create window and renderer - required to create font texture
    std::shared_ptr<SDL_Window> window;
    const auto windowId(sdlResourceManager->createWindow(window));

    std::shared_ptr<SDL_Renderer> renderer(sdlResourceManager->getWindowRenderer(windowId));


    std::cout << "loading font" << std::endl;



    
    std::cout << "init font manager..." << std::endl;
    FontManager fontManager;
    std::cout << "font manager initialized" << std::endl;

    std::string fontFilenameLiberationMono;
    # if FONT_TIMES_NEW_ROMAN
    std::string fontFilenameTimesNewRoman;
    #endif

    // Note: needed in main loop code (scope)
    std::string validCharsLiberationMono;

    try
    {


        // TODO: put this in a try-catch block as well?
        // No: Don't need to because it was already inside a try-catch block
        std::string stringLiberationMono("Liberation Mono");
        std::cout << "loadFontTexture: " << stringLiberationMono << std::endl;

        /*std::string fontFilename =*/
            // TODO: change back, calling the c-style argument version
            // SEGFAULT?
            //fontConfigGetFontFilename(stringLiberationMono.c_str());

        fontFilenameLiberationMono = fontConfigGetFontFilename(stringLiberationMono);
        std::cout << "fontFilenameLiberationMono=" << fontFilenameLiberationMono << std::endl;
        fontManager.loadFontTexture(windowId, fontFilenameLiberationMono, 12, ColorPalette::getStatic(ColorName::BLACK), validCharsLiberationMono);


        // This function does the same as the above, however instead of
        // passing a pre-determined full font file path the function
        // argument is a font description to be passed to the fc font
        // config function.
        // This function needs to have some way to signal whether the
        // fc font config function succeeded.
        # if FONT_TIMES_NEW_ROMAN
        std::string stringTimesNewRoman("Times New Roman");
        std::cout << "loadFontTextureFromDescription: " << stringTimesNewRoman << std::endl;

        std::string validCharsTimesNewRoman;
        fontFilenameTimesNewRoman = fontManager.loadFontTextureFromDescription(
            windowId, stringTimesNewRoman, 24, ColorPalette::getStatic(ColorName::BLACK), validCharsTimesNewRoman);

        std::cout << "fontFilenameTimesNewRoman=" << fontFilenameTimesNewRoman << std::endl;
        #endif

        // testing the case of a font that does not exist
        std::string stringInvalid("nothing");
        std::cout << "loadFontTextureFromDescription: " << stringInvalid << std::endl;
        std::string validCharsInvalid;

        fontManager.loadFontTextureFromDescription(windowId, stringInvalid, 12, ColorPalette::getStatic(ColorName::BLACK), validCharsInvalid);
            // seems to return something random

        std::cout << "validCharsInvalid.size()="
                  << validCharsInvalid.size()
                  << std::endl;
        std::cout << std::endl;
    }
    catch(const SDLLibException &e)
    {
        std::cout << e.what() << std::endl;
    }
    catch(const std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
    catch(...)
    {
        std::cout << "catch ... case" << std::endl;
        throw;
    }
    // catch fontManager throws here
    // catch fontConfig throws here
    // TODO: these catch statements should be changed
    // if the font tries to load a character which cannot be rendered
    // this causes a throw, and this throw should be visible here, not
    // caught and silently ignored (ok its not slient because cout
    // prints e.what() but this is near slient among all the other output)

    std::cout << "GetFontTexture: " << fontFilenameLiberationMono << " 12" << std::endl;
    std::shared_ptr<FontTexture> fontTextureLiberationMono = fontManager.getFontTexture(
        windowId, fontFilenameLiberationMono, 12, ColorPalette::getStatic(ColorName::BLACK));


    // load a larger version of Liberation Mono font
    std::string validCharsLiberationMonoLarge;
    try
    {
        std::cout << "loadFontTexture: " << "Liberation Mono (24)" << std::endl;
        fontManager.loadFontTexture(windowId, fontFilenameLiberationMono, 24, ColorPalette::getStatic(ColorName::BLACK), validCharsLiberationMonoLarge);
        std::cout << std::endl;
    }
    catch(const SDLLibException &e)
    {
        std::cout << e.what() << std::endl;
    }
    catch(const std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
    catch(...)
    {
        std::cout << "catch ... case" << std::endl;
        throw;
    }
    std::cout << "getFontTexture: " << fontFilenameLiberationMono << " 24" << std::endl;
    std::shared_ptr<FontTexture> fontTextureLiberationMonoLarge(
        fontManager.getFontTexture(windowId, fontFilenameLiberationMono, 24, ColorPalette::getStatic(ColorName::BLACK)));

    # if FONT_TIMES_NEW_ROMAN
    std::cout << "getFontTexture: " << fontFilenameTimesNewRoman << " 24" << std::endl;
    std::shared_ptr<FontTexture> fontTextureTimesNewRoman(
        fontManager.getFontTexture(windowId, fontFilenameTimesNewRoman, 24, ColorPalette::getStatic(ColorName::BLACK)));
    #endif


    // API sketch
    //SDL_TextGridWindow textgridwindow = sdlManager.CreateTextGridWindow();
    //textgridwindow.SetFont(const FontTexture &font_manager_liberation_mono)


/*
    TextGrid textgrid(font_liberation_mono);
    textgrid.SetFont(font_liberation_mono);
*/

    //SDL_Color COLOR_BLACK = SDL_Color(0, 0, 0);
    //SDL_Color COLOR_TEXT_DEFAULT = COLOR_BLACK;


    // continue to render stuff
    SDL_Color COLOR_BACKGROUND = ColorPalette::getStatic(ColorName::WHITE);

    std::cout << "Main loop" << std::endl;

    int X = 0;

    // main infinite loop
    for(bool quit = false; quit == false; )
    {

        SDL_Event event;
        while(SDL_PollEvent(&event) != 0)
        {
            if(event.type == SDL_QUIT)
            {
                quit = true;
            }
            else
            {
                // nothing
            }
        }


        SDL_SetRenderDrawColor(renderer.get(), COLOR_BACKGROUND);
        SDL_RenderClear(renderer.get());

        debugDrawCharsTexture(
            renderer,
            fontTextureLiberationMono,
            X, 500);
        -- X;
        if(X < -1000) X = 0;
        SDL_Delay(10);


        //textgrid.Draw(window);

        int fontLineSkip =
            fontTextureLiberationMono->getFontLineSkip();
            
        int fontAscent =
            fontTextureLiberationMono->getFontAscent();

        int pos_x = 200;
        int pos_y = 20;

        std::string myText("hello world 0123456789'''");
        int t_pos_x = pos_x;
        for(char c: myText)
        {
            write(renderer,
                fontTextureLiberationMono,
                c, t_pos_x, pos_y,
                true);
        }

        writeString(
            renderer,
            fontTextureLiberationMonoLarge,
            "some random text", t_pos_x, pos_y, true);

        t_pos_x = pos_x;
        for(char c: myText)
        {
            writeWithBackground(
                renderer,
                fontTextureLiberationMono,
                c, t_pos_x, pos_y + 40,
                true,
                ColorPalette::getStatic(ColorName::GREEN));
        }

        // create a test string with all possible ASCII symbols
        t_pos_x = pos_x;
        std::string allSymbols = validCharsLiberationMonoLarge;
        std::vector<std::string> vAllSymbols;
        std::string vAllSymbolsNext;
        for(auto c: allSymbols)
        {
            if(vAllSymbolsNext.size() < 10)
            {
                vAllSymbolsNext.push_back(c);
            }
            else
            {
                vAllSymbols.push_back(vAllSymbolsNext);
                vAllSymbolsNext.clear();
            }
        }
        if(vAllSymbolsNext.size())
        {
            vAllSymbols.push_back(vAllSymbolsNext);
            vAllSymbolsNext.clear();
        }

        auto t_pos_y = 20;
        for(const auto &allSymbols: vAllSymbols)
        {
            t_pos_x = 20;

            writeStringWithTickTockBackground(renderer,
                fontTextureLiberationMonoLarge,
                allSymbols,
                t_pos_x, t_pos_y, true,
                ColorPalette::getStatic(ColorName::RED),
                ColorPalette::getStatic(ColorName::GREEN));

            t_pos_y += fontTextureLiberationMonoLarge->getFontLineSkip();
        }


        // back to older code
        # if FONT_TIMES_NEW_ROMAN
        std::string mystring("this is a test string!");
        t_pos_x = pos_x;
        writeString(
            renderer,
            fontTextureTimesNewRoman,
            mystring, t_pos_x, pos_y + 80,
            false);
        #endif
    

        SDL_RenderPresent(renderer.get());


    } // for



}