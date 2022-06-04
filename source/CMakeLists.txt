add_subdirectory(utils)
add_subdirectory(enviroment)

set(CORE_SOURCE_FILES
	game_context.hpp
	game_module_interface.hpp
	player_control_module.hpp
	player_control_module.cpp
	map_loading_module.hpp
	map_loading_module.cpp
	camera.hpp
	camera.cpp
	block_side.hpp
	game.hpp
	game.cpp

	render/opengl_raw_chunk_data.hpp
	render/opengl_chunk.hpp
	render/opengl_chunk.cpp
	render/opengl_map.hpp
	render/opengl_map.cpp
	render/opengl_scene.hpp
	render/opengl_scene.cpp
	render/opengl_render_module.hpp
	render/opengl_render_module.cpp

	model/block.hpp
	model/chunk.hpp

	scene/block_look_at.hpp
	scene/map.hpp
	scene/map.cpp
	scene/scene.hpp
	scene/scene.cpp

	ui/i_imgui_element.hpp
	ui/imgui_text.hpp
	ui/imgui_text.cpp
	ui/imgui_button.hpp
	ui/imgui_button.cpp
	ui/imgui_window.hpp
	ui/imgui_window.cpp
)

add_library(BlocksCore SHARED ${CORE_SOURCE_FILES})
target_include_directories(BlocksCore
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(BlocksCore
	PUBLIC
		BlocksUtils
		BlocksEnviroment

		FastNoise2::FastNoise
)


target_include_directories(BlocksCore
	PRIVATE 
		"${PROJECT_BINARY_DIR}/configs"
)


install(
	IMPORTED_RUNTIME_ARTIFACTS
		glew
		glfw3
		FastNoise2::FastNoise

		BlocksCore
	DESTINATION
		.
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${CORE_SOURCE_FILES})
