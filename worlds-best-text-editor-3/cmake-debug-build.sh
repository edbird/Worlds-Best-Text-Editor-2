#!/bin/bash

cmake --preset=debug-configure -DCMAKE_CXX_COMPILER=g++-14
cmake --build --preset=debug-build -j$(nproc)