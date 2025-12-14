include_directories(../include)

add_library(configLib STATIC
    Config.cpp)

target_include_directories(configLib .)

