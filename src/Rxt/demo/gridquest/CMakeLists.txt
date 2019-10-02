# "gridquest" demo

add_executable(gridquest main.cpp)
# add_executable(gridquest main.cpp main_context.hpp space.hpp)
target_link_libraries(gridquest RxtGraphics fmt::fmt)
target_include_directories(gridquest PUBLIC ${PROJECT_SOURCE_DIR}/external)
