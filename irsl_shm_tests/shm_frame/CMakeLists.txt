cmake_minimum_required(VERSION 3.0.0)

project(irsl_hardware_controller_tests)

set(CMAKE_BUILD_TYPE RelWithDebInfo)

find_package(irsl_shm_controller REQUIRED)
find_package(irsl_realtime_utils  REQUIRED)

### debug print variables
##get_cmake_property(_variableNames CACHE_VARIABLES)
#get_cmake_property(_variableNames VARIABLES)
#message("VARIABLES>>>>")
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#  message("${_variableName}=${${_variableName}}")
#endforeach()
#message("VARIABLES<<<<")

add_executable(test_host   test_host.cpp)
add_executable(test_client test_client.cpp)

# include_directories(
target_link_libraries(test_host   irsl_shm_controller)
target_link_libraries(test_client irsl_shm_controller)
###target_link_libraries(test_host   irsl_shm_controller irsl_reamtime_utils)
###target_link_libraries(test_client irsl_shm_controller irsl_reamtime_utils)
