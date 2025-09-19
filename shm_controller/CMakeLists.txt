cmake_minimum_required(VERSION 3.0.0)

project(irsl_shm_controller
  VERSION 0.0.1
  LANGUAGES CXX
)

option(USE_DOUBLE_PRECISION "Using double for floating point variables" ON)
option(BUILD_PYBIND11 "Build Python bindings with pybind11" ON)

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

# ----- Python bindings (pybind11) -----
# cmake -S /home/irsl/temp/irsl_hardware_controller/shm_controller -B /home/irsl/temp/irsl_hardware_controller/shm_controller/build -DBUILD_PYBIND11=ON -DCMAKE_BUILD_TYPE=RelWithDebInfo
if(BUILD_PYBIND11)
  include(CheckCXXCompilerFlag)
  # Try to find pybind11 via package config first
  find_package(pybind11 QUIET CONFIG)
  if(NOT pybind11_FOUND)
    message(STATUS "pybind11 not found via find_package; using FetchContent to download it")
    include(FetchContent)
    set(FETCHCONTENT_QUIET OFF)
    FetchContent_Declare(
      pybind11
      GIT_REPOSITORY https://github.com/pybind/pybind11.git
      GIT_TAG v2.12.0
    )
    FetchContent_MakeAvailable(pybind11)
  endif()

  # Build extension module
  set(pymod_name irsl_shm)
  pybind11_add_module(${pymod_name} MODULE
    python/py_irsl_shm.cpp
  )
  target_link_libraries(${pymod_name} PRIVATE ${targetname})
  # Inherit definitions such as IRSL_USE_DOUBLE from top-level options if needed
  if(USE_DOUBLE_PRECISION)
    target_compile_definitions(${pymod_name} PRIVATE IRSL_USE_DOUBLE)
  endif()
  target_include_directories(${pymod_name} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)

  # Optional: install module to lib directory
  install(TARGETS ${pymod_name}
    LIBRARY DESTINATION lib
  )
endif()
