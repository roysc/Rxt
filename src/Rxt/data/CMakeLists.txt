# Data structures library
# Includes wrapper around boost-graph-based tools
set(_HEADERS
    graph.hpp
    _graph_dummy.cpp
)
add_library(RxtData ${_HEADERS})

use_stdlib(RxtData ${_STDLIB_DIR} ${_STDLIB_LIBDIR})
target_include_directories(RxtData PUBLIC
    $<BUILD_INTERFACE:${_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:${_INCLUDE_DEST}>
)

target_link_libraries(RxtData RxtBase)
target_link_libraries(RxtData Boost::graph)
# set(_LIBDIR ${Rxt_LOCAL_PREFIX}/lib)
# target_link_options(RxtData PUBLIC -L${_LIBDIR} -Wl,-rpath,${_LIBDIR})

make_test(graph graph.test.cpp)
target_link_libraries(test_graph RxtData)

install(FILES ${_HEADERS} DESTINATION "${_INCLUDE_DEST}/Rxt/data")
