add_library(colorLib
    #STATIC
        ColorLib.cpp
)

target_include_directories(colorLib
    PUBLIC
        .
)

target_link_libraries(colorLib
    PUBLIC
        sdlHelperLib
)