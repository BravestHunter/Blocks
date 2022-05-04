set(CORE_SOURCE_FILES
	export.h
	compile_utils.hpp

	game.hpp
	game.cpp
	camera.hpp
	camera.cpp
	image.hpp

	platform/glfw_headers.hpp
	platform/cursor_mode.hpp
	platform/glfw_platform.hpp
	platform/glfw_platform.cpp
	platform/glfw_window.hpp
	platform/glfw_window.cpp

	io/file_api.hpp
	io/file_api.cpp

	render/glew_headers.hpp
	render/opengl_render_system.hpp
	render/opengl_render_system.cpp
	render/opengl_buffer.hpp
	render/opengl_buffer.cpp
	render/opengl_vertex_array_object.hpp
	render/opengl_vertex_array_object.cpp
	render/opengl_texture.hpp
	render/opengl_texture.cpp
	render/opengl_texture_array.hpp
	render/opengl_texture_array.cpp
	render/opengl_model.hpp
	render/opengl_model.cpp
	render/opengl_chunk.hpp
	render/opengl_chunk.cpp
	render/opengl_map.hpp
	render/opengl_map.cpp
	render/opengl_scene.hpp
	render/opengl_scene.cpp
	render/opengl_raw_chunk_data.hpp
	render/opengl_shader.hpp
	render/opengl_shader.cpp

	model/block.hpp
	model/chunk.hpp

	scene/map.hpp
	scene/map.cpp
	scene/scene.hpp
	scene/scene.cpp
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
