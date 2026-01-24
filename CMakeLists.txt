cmake_minimum_required(VERSION 3.10)
project(proj)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS off)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

set(CMAKE_AUTOMOC on)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(proj)
file(GLOB_RECURSE sources src/*.cpp include/*.hpp)
target_include_directories(proj PRIVATE include)
target_sources(proj PRIVATE ${sources})
target_link_libraries(proj PRIVATE Qt6::Widgets)
