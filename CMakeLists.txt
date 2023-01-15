cmake_minimum_required(VERSION 3.7)
project(WorldsBestTextEditor)

# Setup C++ Standard / Version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
#set(CMAKE_BUILD_TYPE DEBUG)

# Set default compile flags for GCC
if(CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "GCC detected, adding compile flags")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra")
endif(CMAKE_COMPILER_IS_GNUCXX)

list(APPEND CMAKE_PREFIX_PATH "/home/debian/local/jsoncpp/lib/")

#set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# required to find FindSDL2TTF.cmake
#/home/debian/Worlds-Best-Text-Editor
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}) # change to list append?
message(STATUS "CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")

# Find required packages
find_package(SDL2 REQUIRED)
find_package(SDL2TTF REQUIRED)

message(STATUS "CMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}")
set(JSONCPP_DIR "/home/debian/local/jsoncpp")

find_package(PkgConfig REQUIRED)

pkg_search_module(JSONCPP jsoncpp)

#find_package(JSONCPP REQUIRED)
#pkg_check_modules(JSONCPP jsoncpp)

#if(JSONCPP_FOUND)
#    message(INFO "Json library found")
#else()
#    message(WARNING "Json library is not found")
#
#    pkg_check_modules(jsoncpp REQUIRED IMPORTED_TARGET jsoncpp)
#    message(STATUS "jsoncpp_FOUND: ${json_FOUND}")
#endif()

include_directories(WorldsBestTextEditor include ${SDL2_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIR}) # ${JSONCPP_INCLUDE_DIR})
#include_directories(~/Git/Program-Arguments)


# Build directories containing libraries for TextGraphicsLibTestExe
add_subdirectory(FontConfigHelper)
add_subdirectory(ResourceLib)
add_subdirectory(ColorLib)
add_subdirectory(FontLib)
add_subdirectory(ExceptionLib)

add_executable(TextGraphicsLibTestExe
    main2.cpp
)

target_link_libraries(TextGraphicsLibTestExe
    PRIVATE
        resourceLib
        fontLib     # my Font Library
        textGraphicsLib
        #fontConfigHelperLib
        #exceptionLib
        fontconfig  # Linux Font Config library
        ${SDL2_LIBRARIES}
        ${SDL2TTF_LIBRARY}
)

configure_file(Version.h.in Version.h)


# Build directories contianing libraries for TextAreaTestExe



add_executable(TextAreaTestExe
    main1.cpp
)

target_link_libraries(TextAreaTestExe
    PUBLIC
        textAreaLib
)


#add_executable(WorldsBestTextEditor
#    src/main.cpp
#    src/Functions.cpp
#    src/FunctionCallback.cpp
#    src/GUIObject.cpp
#    src/Buffer.cpp
#    src/Textbox.cpp
#    src/Textbox_Draw.cpp
#    src/Textbox_Scroll.cpp
#    src/Textbox_Cursor.cpp
#    src/Actions.cpp
#)

#target_link_libraries(WorldsBestTextEditor
#    PUBLIC
#        ${SDL2_LIBRARIES}
#        ${SDL2TTF_LIBRARY}
#        ${JSONCPP_LIBRARIES}
#        PkgConfig::jsoncpp
#)
