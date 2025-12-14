#!/bin/bash

cmake --preset=release-configure -DCMAKE_CXX_COMPILER=g++-14
cmake --build --preset=release-build -j$(nproc)