cmake_minimum_required(VERSION 3.10)

# Nama project
project(RayTracingInOneWeek LANGUAGES CXX)

# Versi standar C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

# Tambahkan executable dari main.cpp
add_executable(raytracer src/main.cpp)