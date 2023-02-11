add_executable(GuiTestExe
    guiMainTest.cpp
)

target_link_libraries(GuiTestExe
    PRIVATE
        resourceLib
        graphicalUserInterfacelib
        #fontLib     # my Font Library
        textGraphicsLib
        #fontConfigHelperLib
        #exceptionLib
        fontconfig  # Linux Font Config library
        ${SDL2_LIBRARIES}
        ${SDL2TTF_LIBRARY}
)