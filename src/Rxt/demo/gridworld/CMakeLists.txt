# "gridworld" demo

add_executable(gridworld main.cpp main_context.hpp space.hpp)
target_link_libraries(gridworld RxtGraphics fmt::fmt)
target_include_directories(gridworld PUBLIC ${PROJECT_SOURCE_DIR}/external)
