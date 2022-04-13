# Add this as existing findGlew kinda can't cope with dynamic lib version...
# TODO: remove this file, find other way to add dll to <TARGET_RUNTIME_DLLS>

# Search for the header
FIND_PATH(GLEW_INCLUDE_DIR "GL/glew.h"
	PATHS
		${GLEW_ROOT}
)

# Search for the library
FIND_LIBRARY(GLEW_LIBRARY
	NAMES 
		glew glew32
	PATHS
		"${GLEW_ROOT}/lib/Release/x64"
)

# Search for the dll
FIND_FILE(GLEW_DLL "glew32.dll"
	PATHS
		"${GLEW_ROOT}/bin/Release/x64"
)

# Add imported library
add_library(glew SHARED IMPORTED)
set_property(TARGET glew PROPERTY
	IMPORTED_LOCATION ${GLEW_DLL}
)
set_property(TARGET glew PROPERTY
	IMPORTED_IMPLIB ${GLEW_LIBRARY}
)
target_include_directories(glew
    INTERFACE
        ${GLEW_INCLUDE_DIR}
)
