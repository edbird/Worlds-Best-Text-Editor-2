
add_library(
    spdlog_util
    INTERFACE
)

target_include_directories(
    spdlog_util
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    spdlog_util
    INTERFACE
    spdlog::spdlog
)
