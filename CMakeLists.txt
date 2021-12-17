cmake_minimum_required(VERSION 3.21)
project(PIRV_3)

find_package(MPI REQUIRED)
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_COMPILER /usr/bin/mpic++)

add_executable(PIRV_3 main.cpp)
target_link_libraries(PIRV_3 PUBLIC MPI::MPI_CXX)
