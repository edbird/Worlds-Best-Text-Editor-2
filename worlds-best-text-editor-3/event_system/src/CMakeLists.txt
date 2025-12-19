add_library(
    event_system
    STATIC
    command.cpp
    keybinding_configuration.cpp
    event_system.cpp
)

target_include_directories(
    event_system
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    event_system
    PUBLIC
    spdlog_util
)