cmake_minimum_required(VERSION 3.7)

project(TextArea VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
#set(CMAKE_BUILD_TYPE DEBUG)

# Set default compile flags for GCC
if(CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "GCC detected, adding compile flags")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra")
endif(CMAKE_COMPILER_IS_GNUCXX)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}) #/home/ecb/Worlds-Best-Text-Editor-3/Text-Grid)

find_package(SDL2 REQUIRED)
find_package(SDL2TTF REQUIRED)


#add_library(textGraphicsLib SHARED IMPORTED)
#set_target_properties(textGraphicsLib PROPERTIES IMPORTED_LOCATION /home/debian/Git/Text-Graphics-Lib/libtextGraphicsLib.a)

list(APPEND CMAKE_PREFIX_PATH "/home/debian/Git/Text-Graphics-Lib/build")
find_library(TEXT_GRAPHICS_LIB textGraphicsLib)


# build text graphics libarary first
add_subdirectory(TextArea)


add_executable(textGridExe
    main.cpp
)

configure_file(version.hpp.in version.hpp)

# build the executable test program
target_link_libraries(textGridExe
    PRIVATE
        ${TEXT_GRAPHICS_LIB}
        textAreaLib
        ${SDL2_LIBRARIES}
        ${SDL2TTF_LIBRARY}
        fontconfig
)
