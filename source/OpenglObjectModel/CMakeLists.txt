set(SOURCE_FILES
	glew_headers.hpp
	object.hpp
	object.cpp
	buffer.hpp
	buffer.cpp
	vertex_array_object.hpp
	vertex_array_object.cpp
	texture_2d.hpp
	texture_2d.cpp
	texture_2d_array.hpp
	texture_2d_array.cpp
	shader.hpp
	shader.cpp
	shader_program.hpp
	shader_program.cpp
)

add_library(OpenglObjectModel STATIC ${SOURCE_FILES})
target_include_directories(OpenglObjectModel
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(OpenglObjectModel
	PUBLIC
		OpenGL::GL
		OpenGL::GLU
		glew

		BlocksUtils
)


source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES})
