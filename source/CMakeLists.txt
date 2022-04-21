set(CORE_SOURCE_FILES
	export.h

	game.hpp
	game.cpp

	platform/glfw_headers.hpp
	platform/glfw_platform.hpp
	platform/glfw_platform.cpp
	platform/glfw_window.hpp
	platform/glfw_window.cpp

	shader.hpp
	shader.cpp
	camera.hpp
	camera.cpp
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
