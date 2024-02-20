cmake_minimum_required(VERSION 3.0.0)

project(irsl_common_utils
  VERSION 0.0.1
  LANGUAGES CXX
)

# set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE RelWithDebInfo)

set(targetname irsl_common_utils)

#### export headerr-only
## ${PROJECT_SOURCE_DIR}/include will be installed
add_library(${targetname} INTERFACE)
target_include_directories(${targetname} INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

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
  PUBLIC_HEADER DESTINATION include COMPONENT Development
  BUNDLE        DESTINATION bin COMPONENT Runtime
)

install(EXPORT ${targetname}Targets
  FILE ${targetname}Config.cmake
  DESTINATION lib/cmake/${targetname}
)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include)


#### export header-only using .cmake.in
### .cmake.in should be below lines
# @PACKAGE_INIT@
# include("${CMAKE_CURRENT_LIST_DIR}/irsl_common_utilsTargets.cmake")
# check_required_components("@PROJECT_NAME@")
###
#add_library(${targetname} INTERFACE)
#target_include_directories(${targetname} INTERFACE
#  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
#  $<INSTALL_INTERFACE:include>
#)
#include(CMakePackageConfigHelpers)
#write_basic_package_version_file(
#  "${PROJECT_BINARY_DIR}/${targetname}ConfigVersion.cmake"
#  VERSION 0.1
#  COMPATIBILITY AnyNewerVersion
#)
#install(TARGETS ${targetname}
#  EXPORT ${targetname}Targets
#  LIBRARY       DESTINATION lib COMPONENT Runtime
#  ARCHIVE       DESTINATION lib COMPONENT Development
#  RUNTIME       DESTINATION bin COMPONENT Runtime
#  PUBLIC_HEADER DESTINATION include COMPONENT Development
#  BUNDLE        DESTINATION bin COMPONENT Runtime
#)
#include(CMakePackageConfigHelpers)
#configure_package_config_file(
#    "${PROJECT_SOURCE_DIR}/${targetname}Config.cmake.in"
#    "${PROJECT_BINARY_DIR}/${targetname}Config.cmake"
#    INSTALL_DESTINATION lib/cmake/${targetname}
#)
#install(EXPORT ${targetname}Targets DESTINATION lib/cmake/${targetname})
#install(FILES "${PROJECT_BINARY_DIR}/${targetname}ConfigVersion.cmake"
#              "${PROJECT_BINARY_DIR}/${targetname}Config.cmake"
#        DESTINATION lib/cmake/${targetname})
#install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include)
