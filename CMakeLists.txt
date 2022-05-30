cmake_minimum_required(VERSION 3.22.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(SO_LAB_3)

file(GLOB_RECURSE SRC_FILES src/*.cpp)

add_executable(main main.cpp ${SRC_FILES})

target_include_directories(main PUBLIC 
${CMAKE_CURRENT_SOURCE_DIR}/include)
