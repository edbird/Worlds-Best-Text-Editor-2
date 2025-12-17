add_library(
    document
    STATIC
    document.cpp
)

target_include_directories(
    document
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    document
    PUBLIC
    spdlog_util
)