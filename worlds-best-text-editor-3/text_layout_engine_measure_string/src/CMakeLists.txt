add_library(
    text_layout_engine_measure_string
    STATIC
    text_layout_engine_measure_string.cpp
    private_functions.cpp
    wrap_line.cpp
)

target_include_directories(
    text_layout_engine_measure_string
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    text_layout_engine_measure_string
    PUBLIC
    spdlog_util
    document
)