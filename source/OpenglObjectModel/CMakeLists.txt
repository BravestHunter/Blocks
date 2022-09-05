set(SOURCE_FILES
	glew_headers.hpp
	opengl_shader.hpp
	opengl_shader.cpp
	opengl_program.hpp
	opengl_program.cpp
	opengl_buffer.hpp
	opengl_buffer.cpp
	opengl_vertex_array_object.hpp
	opengl_vertex_array_object.cpp
	opengl_texture_2d.hpp
	opengl_texture_2d.cpp
	opengl_texture_2d_array.hpp
	opengl_texture_2d_array.cpp
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
