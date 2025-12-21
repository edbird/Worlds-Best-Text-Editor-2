


#include <SDL2/SDL.h>


#include <iostream>


// text grid manages drawing of text to screen as fixed size grid
// text buffer manages file io, buffer, cursor(s), window scrolling,
// text wrapping
// the data inside text grid is filled using data from text buffer
// when it is time to draw


#include "version.h"


#include "TextArea.h"
#include "TextBuffer.h"




#include "SDLManager.h"
#include "SDLResourceMananger.h"
#include "FontManager.h"


#include "SDLHelper.h"


#include "Color.h"






int main(int argc, char *argv[])
{

    std::cout << "Version: " << VERSION_MAJOR
              << " " << VERSION_MINOR << std::endl;

    TextBuffer textbuffer;
    textbuffer.ReadFile("README.md");

    // TextArea moved below because it requires a SDLFontTexture
    // for initialization
    // TODO: move TextBuffer as well


    // SDL stuff copied from TextGraphicsLib folder
    
    SDLManager &sdlManager(SDLManager::getInstance());

    SDLResourceManager sdlResourceManager(sdlManager);



    // create window and renderer - required to create font texture
    std::shared_ptr<SDL_Window> window(
        //sdlResourceManager.CreateWindow(sdlManager));
        sdlResourceManager.CreateWindow());

    std::shared_ptr<SDL_Renderer> renderer(
        sdlResourceManager.GetWindowRenderer());



    std::cout << "init font manager..." << std::endl;
    FontManager fontManager(sdlManager);
    std::cout << "font manager initialized" << std::endl;

    std::string fontFilenameLiberationMono;
    try
    {
        std::string stringLiberationMono("Liberation Mono");

        std::string fontFilename =
            fontConfigGetFontFilename(stringLiberationMono);
        std::cout << "fontFilename=" << fontFilename << std::endl;

        fontFilenameLiberationMono = fontFilename;

        // this function loads a font using the SDL TTF functions
        // taking a font filename (full path) as an argument.
        // The path to the font to be loaded is obtained from the fc
        // font config functions.
        fontManager.LoadFontTexture(renderer, fontFilenameLiberationMono, 12);
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

    // the to get the actual FontTexture
    // possibly the LoadFontTexture should return the same value
    std::shared_ptr<SDLFontTexture> fontTextureLiberationMono =
        fontManager.GetFontTexture(
            fontFilenameLiberationMono, 12);
        // return a shared pointer; good, can pass this to other
        // functions and classes


    // TODO: this constructor call makes the SetFont function
    // redundant
    //TextArea textArea(40, 20, fontTextureLiberationMono);
    //TextArea textArea(40, 20, 200, 200, fontTextureLiberationMono);
    int width = 0;
    int height = 0;
    SDL_GetWindowSize(window.get(), &width, &height);
    TextArea textArea(width, height, fontTextureLiberationMono);
    SDL_Color COLOR_TEXTGRID_BACKGROUND = SDL_MakeColor(200, 200, 200);
    //SDL_Color COLOR_TEXTGRID_BACKGROUND = SDL_MakeColor(0xFF, 0x00, 0x00);
    textArea.SetBackgroundColor(COLOR_TEXTGRID_BACKGROUND);
    //textArea.SetFont(fontTextureLiberationMono);

    // here is a potential problem:
    // the textArea size is set assuming fixed width / monospace font
    // the arguments are the number of characters in x and y
    // whereas in reality the arguments should be the pixel size of
    // the drawable textArea area
    // otherwise, when using different font sizes or non-monospace fonts
    // the drawable area of the textArea scales (changes) rather than
    // remaining static

    // TODO: removed this to test base functions
    fillTextGridFromTextBuffer(textArea, textbuffer);
    

    // TODO: need to change the X and Y around so the indices
    // are row, column
    /*
    textArea.Put(0, 0, '0');
    textArea.Put(1, 0, '1');
    textArea.Put(2, 0, '2');
    textArea.Put(3, 0, '3');
    textArea.Put(4, 0, '4');

    textArea.PutString(0, 1, "hello wor.");
    textArea.PutString(0, 2, "hello world");
    textArea.PutString(0, 3, "hello world");

    textArea.Put(4, 4, 'X');
    textArea.PutString(0, 4, "sho");
    */

    /*
    // find a font file
    const int FONT_FILENAME_BUFFER_SIZE = 4096;
    char font_filename_buffer[FONT_FILENAME_BUFFER_SIZE];
    fontConfigGetFontFilename(font_filename_buffer,
        FONT_FILENAME_BUFFER_SIZE, "Liberation Mono");

    std::string fontFilename(font_filename_buffer); 
    std::cout << "Matched font filename: " << fontFilename << std::endl;


    // open font with font texture manager
    SDLFontTexture font_manager_liberation_mono(fontFilename);
    */

    // TODO:
    //
    // font_manager_liberation_mono should either be allocated
    // using a smart pointer (it should be a smart pointer)
    // or the fonttexturemanager class should be a singleton
    // which manages a set of shared pointers, one for each
    // loaded font
    //
    // NOTE: this is now done, but fonttexturemanager is not
    // a singleton (no reason for it to be)
    //
    // prefer to avoid singletons -
    // therefore make the font_manager_liberatrion_mono a
    // smart pointer or alternatively wrap the SDL_Texture
    // for the font in its own class which manages the
    // copying of the texture from the SDL_Texture to
    // the SDL_Window SDL_Surface
    //



    
    // rendering code block
    {


        // continue to render stuff
        //SDL_Color COLOR_WHITE = SDL_Color(255, 255, 255);
        SDL_Color COLOR_BACKGROUND = COLOR_WHITE;


        



        //TextArea textArea(font_liberation_mono);
        //textArea.SetFont(font_liberation_mono);
        //textArea.SetFont(font_manager_liberation_mono);
        // NOTE: moved
        //textArea.Draw(window);


        textArea.Print(std::cout);
        

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
                else if(event.type == SDL_WINDOWEVENT)
                {
                    if(event.window.event == SDL_WINDOWEVENT_RESIZED)
                    {
                        std::cout << "SDL_WINDOWEVENT_RESIZED" << std::endl;
                        std::cout << event.window.windowID << " "
                                  << event.window.data1 << " "
                                  << event.window.data2 << std::endl;
                    }
                    
                    // note: new if here, because both events seem to fire
                    // simultaniously
                    if(event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                    {
                        std::cout << "SDL_WINDOWEVENT_SIZE_CHANGED" << std::endl;
                        std::cout << event.window.windowID << " "
                                  << event.window.data1 << " "
                                  << event.window.data2 << std::endl;

                        const int width = event.window.data1;
                        const int height = event.window.data2;
                        textArea.SetSizePixels(width, height);

                        // repopulate the textArea
                        // TODO: remove this?
                        fillTextGridFromTextBuffer(textArea, textbuffer);
                    }
                    else
                    {
                        // nothing ?
                    }
                }
                else
                {
                    // nothing
                }
            }




            SDL_SetRenderDrawColor(renderer.get(), COLOR_BACKGROUND);
            SDL_RenderClear(renderer.get());

            //textArea.Draw(renderer);
            textArea.drawPixelSize(renderer);

            SDL_RenderPresent(renderer.get());
        }

    }




    return 0;
}
