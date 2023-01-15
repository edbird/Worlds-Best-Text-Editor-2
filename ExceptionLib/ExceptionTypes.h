#ifndef EXCEPTION_TYPES_H
#define EXCEPTION_TYPES_H


#include <stdexcept>
#include <string>


class SDLLibException : public std::runtime_error
{

public:

    SDLLibException(const std::string &what);

    virtual
    ~SDLLibException();

};


class TTFLibException : public std::runtime_error
{

public:

    TTFLibException(const std::string &what);

    virtual
    ~TTFLibException();

};


#endif // EXCEPTION_TYPES_H