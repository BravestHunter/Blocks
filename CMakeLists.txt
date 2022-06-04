cmake_minimum_required(VERSION 3.23)
project(Blocks VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "target")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

set(BUILD_STATIC_LIBS OFF)
set(BUILD_SHARED_LIBS ON)


set(CONFIGS_DIR "${PROJECT_BINARY_DIR}/configs")
configure_file(config.h.in "configs/config.h")

add_subdirectory(resources)
configure_file(resourceConfig.h.in "configs/resourceConfig.h")


add_subdirectory(external)
find_package(OpenGL REQUIRED)
find_package(GLFW3 3.3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLM REQUIRED)
find_package(Freetype REQUIRED)
find_package(FastNoise2 REQUIRED)

add_subdirectory(source)


add_executable(Blocks "source/main.cpp")
target_link_libraries(Blocks PRIVATE BlocksCore)


# Copy dlls
add_custom_command(TARGET Blocks POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
		$<TARGET_RUNTIME_DLLS:Blocks>
		$<TARGET_FILE_DIR:Blocks>
        COMMAND_EXPAND_LISTS
)

# Copy resources
add_custom_command(TARGET Blocks POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${CMAKE_SOURCE_DIR}/resources"
		"$<TARGET_FILE_DIR:Blocks>/resources"
)
add_custom_command(TARGET Blocks POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${CMAKE_SOURCE_DIR}/resources"
		"$<TARGET_FILE_DIR:Blocks>/../resources" # Copy to IDE project root, so it will find resources properly when executed from IDE
)


install(
	TARGETS
		Blocks
	DESTINATION
		.
)


include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_VERSION_MAJOR "${Blocks_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${Blocks_VERSION_MINOR}")
set(CPACK_SOURCE_GENERATOR "TGZ")
include(CPack)
