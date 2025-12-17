add_library(
    window_geometry
    STATIC
    window_geometry.cpp
)

target_include_directories(
    window_geometry
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    window_geometry
    PUBLIC
    spdlog_util
    document
)