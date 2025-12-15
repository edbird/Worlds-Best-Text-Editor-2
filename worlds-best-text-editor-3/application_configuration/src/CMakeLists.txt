add_library(
    application_configuration
    STATIC
    application_configuration.cpp
)

target_include_directories(
    application_configuration
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    application_configuration
    PUBLIC
    spdlog_util
    JsonCpp::JsonCpp
)