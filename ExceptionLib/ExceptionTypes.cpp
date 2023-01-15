#include "ExceptionTypes.h"

SDLLibException::SDLLibException(const std::string &what)
    : std::runtime_error(what)
{

}

SDLLibException::~SDLLibException()
{

}


TTFLibException::TTFLibException(const std::string &what)
    : std::runtime_error(what)
{

}

TTFLibException::~TTFLibException()
{

}
