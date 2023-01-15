#!/bin/bash
g++ -std=c++20 main.cpp sdlfontmanager.cpp sdlfonttexturemanager.cpp -lSDL2 -lSDL2_ttf -lfontconfig
#g++ -std=c++20 main.cpp `sdl-config --cflags` -lfontconfig
#g++ -o a.out main.o `sdl-config --libs` -lSDL_ttf -lfontconfig
