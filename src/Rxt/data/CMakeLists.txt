# Data structures library

set(_HEADERS
    graph.hpp
    _graph_dummy.cpp
)

add_library(RxtData ${_HEADERS} ${_SOURCES})
_add_component(Data)

target_link_libraries(RxtData PUBLIC ${RXT_BASE_LIBRARY})
_link_boost(RxtData graph)

make_test(graph graph.test.cpp)
target_link_libraries(test_graph RxtData)

make_test(grid grid.test.cpp)
target_link_libraries(test_grid RxtData)

install(FILES ${_HEADERS}
    DESTINATION "${_INCLUDE_DEST}/Rxt/data"
    COMPONENT Data)

_install_component(Data)
