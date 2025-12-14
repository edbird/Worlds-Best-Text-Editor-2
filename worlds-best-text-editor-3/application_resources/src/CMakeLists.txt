add_library(
    application_resources
    STATIC
    application_resources.cpp
)

target_include_directories(
    application_resources
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    application_resources
    PUBLIC
    spdlog_util
    SDL3::SDL3
    SDL3_ttf::SDL3_ttf
)