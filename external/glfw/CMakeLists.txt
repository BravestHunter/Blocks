# Specify binaries location
set (GLFW3_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/bin")

# Try to find library
find_package(GLFW3 3.3 REQUIRED)

# Add imported library
add_library(glfw3 SHARED IMPORTED GLOBAL)
set_property(TARGET glfw3 PROPERTY
	IMPORTED_LOCATION ${GLFW3_LIBRARY}
	)
set_property(TARGET glfw3 PROPERTY
	IMPORTED_IMPLIB ${GLFW3_LIBRARY}
	)
target_include_directories(glfw3
    INTERFACE
        ${GLFW3_INCLUDE_DIR}
)
