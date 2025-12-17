cmake_minimum_required(VERSION 3.10)

project(worlds-best-text-editor-3)

#find_package(fmt CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)

add_subdirectory(spdlog_util)
add_subdirectory(application_configuration)
add_subdirectory(application_resources)
add_subdirectory(color_util)
add_subdirectory(document)
add_subdirectory(text_layout_engine)

add_executable(
    worlds-best-text-editor-3
    main.cpp
)

target_link_libraries(
    worlds-best-text-editor-3
    PRIVATE
    #fmt::fmt
    spdlog_util
    application_configuration
    application_resources
    color_util
    text_layout_engine
    SDL3::SDL3
    SDL3_ttf::SDL3_ttf
    JsonCpp::JsonCpp
)
