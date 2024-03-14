cmake_minimum_required(VERSION 3.0.0)

project(irsl_hardware_controller_test)

set(CMAKE_BUILD_TYPE RelWithDebInfo)

find_package(irsl_shm_controller REQUIRED)
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
add_executable(leader leader.cpp)
add_executable(follower follower.cpp)
add_executable(read_shm read_shm.cpp)
# include_directories(
target_link_libraries(test irsl_shm_controller)
target_link_libraries(leader irsl_shm_controller)
target_link_libraries(follower irsl_shm_controller)
target_link_libraries(read_shm irsl_shm_controller)
