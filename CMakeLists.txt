cmake_minimum_required(VERSION 3.20)
project(Blocks VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

configure_file(config.h.in config.h)


add_executable(Blocks main.cpp)

target_include_directories(Blocks PUBLIC "${PROJECT_BINARY_DIR}")
