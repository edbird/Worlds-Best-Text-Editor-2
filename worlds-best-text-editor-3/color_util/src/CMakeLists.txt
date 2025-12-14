add_library(
    color_util
    STATIC
    color_util.cpp
)

target_include_directories(
    color_util
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    color_util
    PUBLIC
    spdlog_util
)