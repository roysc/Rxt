#!/bin/bash

# Configuration variables for CMake
LOCAL_PREFIX="$HOME/.local"
Boost_DIR="$LOCAL_PREFIX/include"
CGAL_DIR="$HOME/fork/CGAL/build"

CMD="cmake \
   -D Rxt_LOCAL_PREFIX=${LOCAL_PREFIX}\
   -D Boost_DIR=${Boost_DIR}\
   -D CGAL_DIR=${CGAL_DIR}\
   -D CMAKE_INSTALL_PREFIX=${LOCAL_PREFIX}\
   $@"

echo "Running: $CMD"
$CMD
