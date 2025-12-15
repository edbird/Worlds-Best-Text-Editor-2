#!/bin/bash

cmake --preset=debug-configure-ninja -DCMAKE_CXX_COMPILER=g++-14
cmake --build --preset=debug-build-ninja -j$(nproc)