add_library(textAreaLib
    TextArea.cpp
    TextBuffer.cpp
)

target_include_directories(textAreaLib
    PUBLIC
        .
    PRIVATE
        ${SDL2_INCLUDE_DIRS}
        ${SDL2TTF_INCLUDE_DIR}
)

