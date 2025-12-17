add_library(
    text_layout_engine
    STATIC
    text_layout_engine.cpp
)

target_include_directories(
    text_layout_engine
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    text_layout_engine
    PUBLIC
    spdlog_util
    document
)