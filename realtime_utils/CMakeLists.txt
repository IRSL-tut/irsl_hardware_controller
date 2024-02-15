cmake_minimum_required(VERSION 3.0.0)

project(irsl_realtime_utils)

set(CMAKE_BUILD_TYPE RelWithDebInfo)

set(targetname irsl_realtime_utils)

add_library( ${targetname} INTERFACE)

set(irsl_public_headers
  include/irsl/realtime_task.h
)
set_target_properties(${targetname} PROPERTIES
  PUBLIC_HEADER "${irsl_public_headers}"
)

install(TARGETS ${targetname}
  EXPORT irsl-hw-rt-export
#  LIBRARY       DESTINATION lib
#  INCLUDES      DESTINATION include
  PUBLIC_HEADER DESTINATION include/irsl
)

install(EXPORT irsl-hw-rt-export
  FILE ${targetname}-config.cmake
  DESTINATION share/${targetname}/cmake ## share/irsl
  EXPORT_LINK_INTERFACE_LIBRARIES
)

## TODO version
