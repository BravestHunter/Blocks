set(CORE_SOURCE_FILES
	export.h
	compile_utils.hpp

	game.hpp
	game.cpp
	camera.hpp
	camera.cpp
	block_side.hpp

	platform/glfw_headers.hpp
	platform/cursor_mode.hpp
	platform/glfw_platform.hpp
	platform/glfw_platform.cpp
	platform/glfw_window.hpp
	platform/glfw_window.cpp

	io/file_api.hpp
	io/file_api.cpp

	resource/image.hpp
	resource/block_info.hpp
	resource/block_set.hpp
	resource/block_set.cpp
	resource/resource_base.hpp
	resource/resource_base.cpp

	render/glew_headers.hpp
	render/opengl_shader.hpp
	render/opengl_shader.cpp
	render/opengl_program.hpp
	render/opengl_program.cpp
	render/opengl_buffer.hpp
	render/opengl_buffer.cpp
	render/opengl_vertex_array_object.hpp
	render/opengl_vertex_array_object.cpp
	render/opengl_texture_2d.hpp
	render/opengl_texture_2d.cpp
	render/opengl_texture_2d_array.hpp
	render/opengl_texture_2d_array.cpp
	render/opengl_raw_chunk_data.hpp
	render/opengl_chunk.hpp
	render/opengl_chunk.cpp
	render/opengl_map.hpp
	render/opengl_map.cpp
	render/opengl_scene.hpp
	render/opengl_scene.cpp
	render/opengl_render_system.hpp
	render/opengl_render_system.cpp

	model/block.hpp
	model/chunk.hpp

	scene/block_look_at.hpp
	scene/map.hpp
	scene/map.cpp
	scene/scene.hpp
	scene/scene.cpp

	collision/aabb.hpp
	collision/ray.hpp
	collision/ray_intersection_point.hpp
	collision/collisions_api.hpp
	collision/collisions_api.cpp

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
		OpenGL::GL
		OpenGL::GLU
		glew
		glfw3
		glm
		stb
		Freetype::Freetype
		imgui
		FastNoise2::FastNoise
		nlohmann_json
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
