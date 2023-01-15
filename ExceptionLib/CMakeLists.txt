add_library(exceptionLib STATIC
    ExceptionTypes.cpp
)

target_include_directories(exceptionLib
    PUBLIC
        .
)
