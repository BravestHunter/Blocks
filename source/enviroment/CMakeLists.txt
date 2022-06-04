set(SOURCE_FILES
	enviroment_system_interface.hpp
	environment.hpp
	environment.cpp

	platform/cursor_mode.hpp
	platform/glfw_headers.hpp
	platform/glfw_window.hpp
	platform/glfw_window.cpp
	platform/glfw_platform.hpp
	platform/glfw_platform.cpp

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
	render/opengl_context.hpp
	render/opengl_render_system.hpp
	render/opengl_render_system.cpp
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

		OpenGL::GL
		OpenGL::GLU
		glew
		glfw3
		stb
		Freetype::Freetype
		imgui
		nlohmann_json
)


source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES})