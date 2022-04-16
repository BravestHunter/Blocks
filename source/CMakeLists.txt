set(CORE_SOURCE_FILES
	export.h

	game.hpp
	game.cpp

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
	PRIVATE
		OpenGL::GL
		OpenGL::GLU
		glew
		glfw3
		glm
		stb
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
