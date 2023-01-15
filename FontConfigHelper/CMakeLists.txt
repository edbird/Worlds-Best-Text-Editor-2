add_library(fontConfigHelperLib STATIC
    FontConfigHelper.cpp
)

target_include_directories(fontConfigHelperLib
    PUBLIC
        .
)

target_link_libraries(fontConfigHelperLib
    PUBLIC
        fontconfig
)