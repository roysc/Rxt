Rxt
===

Libraries with various **R**esources **+ t**ools for visual games, tools, sims, or other projects.


Contents
--------

  * Data: data structures
  * Geometry: computational geometry (wraps [CGAL](https://www.cgal.org))
  * Graphics: graphics interface & related tools
  * Meta: meta-programming tools


Install
=======

Build with CMake:

    $ mkdir build-rxt && cd build-rxt
    $ cmake ..
    $ make

Build with Emscripten:

    $ source $EMSDK_DIR/emsdk_env.sh
    $ mkdir build-rxt-emscripten && cd build-rxt-emscripten
    $ emcmake cmake .. \
        -D CMAKE_PREFIX_PATH=$HOME/.local_em/lib/x86_64-linux-gnu/cmake/ \
        -D CMAKE_INSTALL_PREFIX=$HOME/.local_em \
        -D DISABLE_FMT=ON
    $ emmake make
