add_library(graphicalUserInterfacelib
    GUIObject.cpp
    GUIBox.cpp
    Label.cpp
    #Statusbar.cpp
    #TextArea.cpp
    #Textbox.cpp
)

target_include_directories(graphicalUserInterfacelib
    PUBLIC
        include
)

target_link_libraries(graphicalUserInterfacelib
    PRIVATE
        jsoncpp
        resourceLib
        sdlHelperLib
        colorLib
        fontLib
)

add_subdirectory(test)