add_library(sdlHelperLib
    STATIC
        SDLHelper.cpp
)

target_include_directories(sdlHelperLib
    PRIVATE
        include
    PUBLIC
        . # TODO : should not expose all header files here!
)

add_library(resourceLib
    STATIC
        SDLManager.cpp
        SDLResourceManager.cpp
        ServiceLocator.cpp
)

target_include_directories(resourceLib
    PRIVATE
        include
    PUBLIC
        .
)

target_link_libraries(resourceLib
    PUBLIC
        exceptionLib
    PRIVATE
        sdlHelperLib
)