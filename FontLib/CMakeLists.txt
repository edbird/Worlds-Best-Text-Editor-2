add_library(fontLib STATIC
    FontManager.cpp
    FontTexture.cpp
)

target_include_directories(fontLib
    PRIVATE
        include
    PUBLIC
        .
)

target_link_libraries(fontLib
    PUBLIC
        fontConfigHelperLib
        resourceLib
        exceptionLib
        colorLib
)


add_library(textGraphicsLib
    WriteFunctions.cpp
)

target_include_directories(textGraphicsLib
    PUBLIC
        ${SDL2_INCLUDE_DIRS}
        ${SDL2TTF_INCLUDE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(textGraphicsLib
    PUBLIC
        SDL2::SDL2
        fontConfigHelperLib
        fontLib
        resourceLib
)