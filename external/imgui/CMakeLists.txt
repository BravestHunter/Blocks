find_package(GLFW3 3.3 REQUIRED)

# Add imported library
add_library(imgui STATIC
	imconfig.h
	imgui.h
	imgui.cpp
	imgui_draw.cpp
	imgui_tables.cpp
	imgui_widgets.cpp
	imgui_demo.cpp
	imgui_internal.h
	imstb_rectpack.h
	imstb_textedit.h
	imstb_truetype.h
	backends/imgui_impl_glfw.h
	backends/imgui_impl_glfw.cpp
	backends/imgui_impl_opengl3.h
	backends/imgui_impl_opengl3.cpp
	backends/imgui_impl_opengl3_loader.h
)
target_include_directories(imgui
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(imgui
	PRIVATE
		glfw3
)
