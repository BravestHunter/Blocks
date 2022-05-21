set(RESOURCE_DIR "resources")
set(SHADERS_DIR "${RESOURCE_DIR}/shaders" PARENT_SCOPE)

install(
	DIRECTORY 
		"${PROJECT_SOURCE_DIR}/resources"
	DESTINATION
		.
)
