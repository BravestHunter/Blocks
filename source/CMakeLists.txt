set(CORE_SOURCE_FILES
	export.h

	game.hpp
	game.cpp
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
