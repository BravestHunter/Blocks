set(SOURCE_FILES
	environment.hpp
	environment.cpp

	task/task.hpp
	task/task.cpp
	task/task_scheduler.hpp
	task/task_scheduler.cpp

	platform/cursor_mode.hpp
	platform/glfw_headers.hpp
	platform/glfw_window.hpp
	platform/glfw_window.cpp
	platform/glfw_platform.hpp
	platform/glfw_platform.cpp

	resource/block_info.hpp
	resource/block_set.hpp
	resource/block_set.cpp
	resource/mesh.hpp
	resource/mesh.cpp
	resource/material.hpp
	resource/model.hpp
	resource/model.cpp
	resource/resource_base.hpp
	resource/resource_base.cpp
)

add_library(BlocksEnviroment STATIC ${SOURCE_FILES})
target_include_directories(BlocksEnviroment
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        "${PROJECT_BINARY_DIR}/configs"
)

target_link_libraries(BlocksEnviroment
	PUBLIC
		BlocksUtils

		glfw3
		stb
		Freetype::Freetype
		imgui
		nlohmann_json
		tinygltf
		
)


source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES})