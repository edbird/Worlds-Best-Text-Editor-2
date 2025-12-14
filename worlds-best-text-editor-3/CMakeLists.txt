cmake_minimum_required(VERSION 3.10)

project(worlds-best-text-editor-3)

#find_package(fmt CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)

add_executable(
    worlds-best-text-editor-3
    main.cpp
)

target_link_libraries(
    worlds-best-text-editor-3
    PRIVATE
    #fmt::fmt
    SDL3::SDL3
)
