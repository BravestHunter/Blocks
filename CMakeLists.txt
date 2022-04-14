cmake_minimum_required(VERSION 3.23)
project(Blocks VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "target")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

set(BUILD_STATIC_LIBS OFF)
set(BUILD_SHARED_LIBS ON)

configure_file(config.h.in "config/config.h")


add_subdirectory(external)
find_package(OpenGL REQUIRED)
find_package(GLFW3 3.3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLM REQUIRED)

add_subdirectory(source)

add_executable(Blocks "source/main.cpp")
target_include_directories(Blocks
	PRIVATE 
		"${PROJECT_BINARY_DIR}/config"
)
target_link_libraries(Blocks PRIVATE BlocksCore)


add_custom_command(TARGET Blocks POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
		$<TARGET_RUNTIME_DLLS:Blocks>
		$<TARGET_FILE_DIR:Blocks>
        COMMAND_EXPAND_LISTS
)
