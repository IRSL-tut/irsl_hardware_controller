cmake_minimum_required(VERSION 3.15)

project(irsl_realtime_utils
  VERSION 0.99.1
  LANGUAGES CXX
)

# set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE RelWithDebInfo)

set(targetname irsl_realtime_utils)

add_library( ${targetname} INTERFACE)

target_include_directories(${targetname} INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(${targetname} INTERFACE rt)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${PROJECT_BINARY_DIR}/${targetname}ConfigVersion.cmake"
  # VERSION 1.0.1
  COMPATIBILITY AnyNewerVersion
)
install(FILES "${PROJECT_BINARY_DIR}/${targetname}ConfigVersion.cmake"
  DESTINATION lib/cmake/${targetname}
)

install(TARGETS ${targetname}
  EXPORT ${targetname}Targets
  LIBRARY       DESTINATION lib COMPONENT Runtime
  ARCHIVE       DESTINATION lib COMPONENT Development
  RUNTIME       DESTINATION bin COMPONENT Runtime
  # PUBLIC_HEADER DESTINATION include COMPONENT Development
  BUNDLE        DESTINATION bin COMPONENT Runtime
)

install(EXPORT ${targetname}Targets
  FILE ${targetname}Config.cmake
  NAMESPACE ${targetname}::
  DESTINATION lib/cmake/${targetname}
)
## header install (by copy)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include)
