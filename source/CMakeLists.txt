set(CORE_SOURCE_FILES
	export.h
	compile_utils.hpp
	
	game.hpp
	game.cpp
	camera.hpp
	camera.cpp

	platform/glfw_headers.hpp
	platform/glfw_platform.hpp
	platform/glfw_platform.cpp
	platform/glfw_window.hpp
	platform/glfw_window.cpp

	render/glew_headers.hpp
	render/opengl_render_system.hpp
	render/opengl_render_system.cpp
	render/opengl_shader.hpp
	render/opengl_shader.cpp
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
)


target_include_directories(BlocksCore
	PRIVATE 
		"${PROJECT_BINARY_DIR}/configs"
)


install(
	IMPORTED_RUNTIME_ARTIFACTS
		glew
		glfw3

		BlocksCore
	DESTINATION
		.
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${CORE_SOURCE_FILES})
