#!/bin/bash
# Helper script to configure CMake with ccache and Ninja
mkdir -p build
cmake -B build -G Ninja \
  -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
  -DCMAKE_C_COMPILER_LAUNCHER=ccache \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
  "$@"
