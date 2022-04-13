cmake_minimum_required(VERSION 3.20)
project(Blocks VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "target")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

configure_file(config.h.in "config/config.h")

add_subdirectory(external)

add_executable(Blocks main.cpp)
target_include_directories(Blocks
	PRIVATE 
		"${PROJECT_BINARY_DIR}/config"
)
target_link_libraries(Blocks PRIVATE glfw3)
