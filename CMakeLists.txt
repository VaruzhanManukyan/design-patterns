cmake_minimum_required(VERSION 4.3)
project(design_patterns_)

set(CMAKE_CXX_STANDARD 20)

add_executable(design_patterns_singleton singleton/main.cpp)
add_executable(design_patterns_builder builder/main.cpp)
add_executable(design_patterns_prototype prototype/main.cpp)
add_executable(design_patterns_factory factory/main.cpp)
add_executable(design_patterns_abstract_factory abstract_factory/main.cpp)
add_executable(design_patterns_composite composite/main.cpp)
add_executable(design_patterns_adapter adapter/main.cpp)
