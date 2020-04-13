# Data structures library

# Depends on boost graph for graph interface. Other possibilities?
find_package(Boost 1.68 REQUIRED COMPONENTS graph)

set(_HEADERS
    graph.hpp
    _graph_dummy.cpp
)

add_library(RxtData ${_HEADERS} ${_SOURCES})
add_library(Rxt::Data ALIAS RxtData)

use_stdlib(RxtData ${_STDLIB_DIR} ${_STDLIB_LIBDIR})
target_include_directories(RxtData PUBLIC
    $<BUILD_INTERFACE:${_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:${_INCLUDE_DEST}>
)

target_link_libraries(RxtData ${Rxt_BASE_LIBRARY} Boost::graph)

make_test(graph graph.test.cpp)
target_link_libraries(test_graph RxtData)

make_test(grid grid.test.cpp)
target_link_libraries(test_grid RxtData)

install(FILES ${_HEADERS}
    DESTINATION "${_INCLUDE_DEST}/Rxt/data"
    COMPONENT Data)

install_component(Data)
