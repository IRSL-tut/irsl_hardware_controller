cmake_minimum_required(VERSION 3.0.0)

project(irsl_shm_controller
  VERSION 0.0.1
  LANGUAGES CXX
)

option(USE_DOUBLE_PRECISION "Using double for floating point variables" ON)

set(CMAKE_BUILD_TYPE RelWithDebInfo)

## find_package(yaml-cpp)
## find_package(boost ...)
set(targetname irsl_shm_controller)

add_library(${targetname} SHARED src/ShmManager.cpp src/ShmSettings.cpp src/ShmDataHeader.cpp src/open_shared_memory.cpp)
include_directories(include)

## target_link_libraries(${targetname} xxx)
add_definitions(-Wno-pointer-arith)
if(USE_DOUBLE_PRECISION)
  add_definitions(-DIRSL_USE_DOUBLE)
endif()

set(irsl_public_headers
  include/irsl/shm_controller.h
)
set_target_properties(${targetname} PROPERTIES
  PUBLIC_HEADER "${irsl_public_headers}"
)

install(TARGETS ${targetname}
  EXPORT irsl-hw-con-export
  LIBRARY       DESTINATION lib
  INCLUDES      DESTINATION include
  PUBLIC_HEADER DESTINATION include/irsl
)

install(EXPORT irsl-hw-con-export
  FILE ${targetname}Config.cmake
  DESTINATION lib/cmake/${targetname}
  EXPORT_LINK_INTERFACE_LIBRARIES
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
