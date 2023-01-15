add_library(resourceLib STATIC
    SDLHelper.cpp
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
)