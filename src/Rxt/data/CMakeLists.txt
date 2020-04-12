# Rxt::Data - data structures & algorithms library

set(_HEADERS
    graph.hpp
    graph_search.hpp
    grid_graph.hpp
#     quadtree.hpp
)
set(_SOURCES _graph_dummy.cpp)

add_library(RxtData ${_HEADERS} ${_SOURCES})
_add_component(Data)

target_link_libraries(RxtData PUBLIC ${RXT_BASE_LIBRARY})
_use_boost(RxtData)

# Tests
set(_TESTS
    graph
    grid
    grid_graph
    quadtree
    graph_search
)
foreach(_TEST ${_TESTS})
    make_test(${_TEST} FILES ${_TEST}.test.cpp LINK RxtData)
endforeach()

_use_boost(test_graph)
_use_boost(test_graph_search)
_use_boost(test_grid_graph)

install(FILES ${_HEADERS}
    DESTINATION "${_INCLUDE_DEST}/Rxt/data"
    COMPONENT Data)

_install_component(Data)
