add_library(
    performance_util
    STATIC
    performance_util.cpp
)

target_include_directories(
    performance_util
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    performance_util
    PUBLIC
    spdlog_util
)