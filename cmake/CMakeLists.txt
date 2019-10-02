cmake_minimum_required(VERSION 3.8)

include(CMakePackageConfigHelpers)

configure_package_config_file(
  "RxtConfig.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/RxtConfig.cmake"
  INSTALL_DESTINATION ${_CMAKE_MODULE_DEST}
  # PATH_VARS _INCLUDE_DEST _LIB_DEST _CMAKE_MODULE_DEST
)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/RxtConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY ExactVersion
)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/RxtConfig.cmake" DESTINATION ${_CMAKE_MODULE_DEST})
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/RxtConfigVersion.cmake" DESTINATION ${_CMAKE_MODULE_DEST})
