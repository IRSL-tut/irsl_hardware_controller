cmake_minimum_required(VERSION 3.0.0)

project(irsl_hardware_controller_tests)

set(CMAKE_BUILD_TYPE RelWithDebInfo)

find_package(irsl_common_utils  REQUIRED)
find_package(yaml-cpp)

### debug print variables
##get_cmake_property(_variableNames CACHE_VARIABLES)
#get_cmake_property(_variableNames VARIABLES)
#message("VARIABLES>>>>")
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#  message("${_variableName}=${${_variableName}}")
#endforeach()
#message("VARIABLES<<<<")

include_directories(${YAML_CPP_INCLUDE_DIR})
add_executable(test_yaml test_yaml.cpp)
target_link_libraries(test_yaml ${YAML_CPP_LIBRARIES} irsl_common_utils)
