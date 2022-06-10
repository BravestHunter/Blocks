set(SOURCE_FILES
	block.hpp
	chunk.hpp
)

add_library(BlocksModel INTERFACE ${SOURCE_FILES})
target_include_directories(BlocksModel
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES})
