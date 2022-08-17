set(SOURCE_FILES
	block.hpp
	chunk.hpp
	chunk_position.hpp
	world.hpp
	serialization.hpp
	serialization.cpp
)

add_library(BlocksModel STATIC ${SOURCE_FILES})
target_include_directories(BlocksModel
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(BlocksModel
	PUBLIC
		glm
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES})
