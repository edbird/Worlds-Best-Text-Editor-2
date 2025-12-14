#ifndef SDLMANAGER_H
#define SDLMANAGER_H


// C++ includes
#include <memory>


class SDLManager
{

public:

    SDLManager();

    static bool initSuccess();

    static void libsDestroy();

private:

    static void libsInit();

};


#endif // SDLMANAGER_H