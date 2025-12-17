add_library(
    text_area
    STATIC
    text_area.cpp
)

target_include_directories(
    text_area
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    text_area
    PUBLIC
    spdlog_util
    text_layout_engine_measure_string
    window_geometry
)