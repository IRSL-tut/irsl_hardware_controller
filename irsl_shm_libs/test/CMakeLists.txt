cmake_minimum_required(VERSION 3.0.0)

project(irsl_hardware_controller_test)

set(CMAKE_BUILD_TYPE RelWithDebInfo)

find_package(irsl_hardware_controller REQUIRED)
## find_package(boost ...)

### debug print variables
##get_cmake_property(_variableNames CACHE_VARIABLES)
get_cmake_property(_variableNames VARIABLES)
message("VARIABLES>>>>")
list (SORT _variableNames)
foreach (_variableName ${_variableNames})
  message("${_variableName}=${${_variableName}}")
endforeach()
message("VARIABLES<<<<")

add_executable(test test.cpp)
# include_directories(
target_link_libraries(test irsl_hardware_controller)
