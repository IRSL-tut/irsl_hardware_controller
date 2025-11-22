cmake_minimum_required(VERSION 3.15)

project(common_test)

find_package(irsl_common_utils)

add_executable(test00 test00.cpp)

target_link_libraries(test00 PRIVATE
  irsl_common_utils::irsl_common_utils
)
