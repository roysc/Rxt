# Rxt::Reflect - reflection utils

set(_HEADERS
    demangle.hpp
)
set(_SOURCES
    demangle.cpp
)

add_library(RxtReflect ${_HEADERS} ${_SOURCES})
_add_component(Reflect)

target_link_libraries(RxtReflect PUBLIC ${RXT_BASE_LIBRARY})

make_test(demangle FILES demangle.test.cpp)
target_link_libraries(test_demangle RxtReflect)

install(FILES ${_HEADERS}
    DESTINATION "${_INCLUDE_DEST}/Rxt/reflect"
    COMPONENT Reflect)

_install_component(Reflect)
