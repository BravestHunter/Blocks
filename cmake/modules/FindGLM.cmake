# Default search dirs
set(_glm_HEADER_SEARCH_DIRS
	"/usr/include"
	"/usr/local/include"
	"${CMAKE_SOURCE_DIR}/includes"
	"C:/Program Files (x86)/glm"
)

# Check environment variable
set(_glm_ENV_ROOT_DIR "$ENV{GLM_ROOT}")
if(NOT GLM_ROOT AND _glm_ENV_ROOT_DIR)
	set(GLM_ROOT "${_glm_ENV_ROOT_DIR}")
endif(NOT GLM_ROOT AND _glm_ENV_ROOT_DIR)

# put user specified location at beginning of search
if(GLM_ROOT)
	SET(_glm_HEADER_SEARCH_DIRS 
			"${GLM_ROOT}"
			"${GLM_ROOT}/include"
			${_glm_HEADER_SEARCH_DIRS}
		)
endif(GLM_ROOT)

# Locate header
find_path(GLM_INCLUDE_DIR "glm/glm.hpp"
	PATHS ${_glm_HEADER_SEARCH_DIRS}
)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLM DEFAULT_MSG GLM_INCLUDE_DIR)
if(GLM_FOUND)
	set(GLM_INCLUDE_DIRS "${GLM_INCLUDE_DIR}")
	message(STATUS "GLM_INCLUDE_DIR = ${GLM_INCLUDE_DIR}")
endif(GLM_FOUND)

# Add library
add_library(glm INTERFACE IMPORTED)
target_include_directories(glm
    INTERFACE
        ${GLM_INCLUDE_DIR}
)