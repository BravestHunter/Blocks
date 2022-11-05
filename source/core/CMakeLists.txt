set(CORE_SOURCE_FILES
	game_context.hpp
	block_side.hpp
	game.hpp
	game.cpp

	time/time_state.hpp
	time/time_module.hpp
	time/time_module.cpp

	input/input_state.hpp
	input/input_module.hpp
	input/input_module.cpp

	ecs/components/transform.hpp
	ecs/components/physics_body.hpp
	ecs/components/ai_component.hpp

	simulation/model_update_event_type.hpp
	simulation/base_model_update_event.hpp
	simulation/player_position_changed_event.hpp
	simulation/entity_physics_body_changed_event.hpp
	simulation/chunk_updated_event.hpp
	simulation/camera.hpp
	simulation/camera.cpp
	simulation/control_mode.hpp
	simulation/entity_spawn_module.hpp
	simulation/entity_spawn_module.cpp
	simulation/entity_ai_module.hpp
	simulation/entity_ai_module.cpp
	simulation/player_control_module.hpp
	simulation/player_control_module.cpp
	simulation/physics_module.hpp
	simulation/physics_module.cpp
	simulation/simulation_module.hpp
	simulation/simulation_module.cpp

	presentation/map_loading_context.hpp
	presentation/presentation_context.hpp
	presentation/map_loading_module.hpp
	presentation/map_loading_module.cpp
	presentation/presentation_module.hpp
	presentation/presentation_module.cpp

	render/opengl_context.hpp
	render/opengl_sprite.hpp
	render/opengl_sprite.cpp
	render/opengl_chunk_vertex.hpp
	render/opengl_chunk_vertex.cpp
	render/opengl_chunk.hpp
	render/opengl_chunk.cpp
	render/opengl_chunk_builder.hpp
	render/opengl_chunk_builder.cpp
	render/opengl_map.hpp
	render/opengl_map.cpp
	render/opengl_model.hpp
	render/opengl_model.cpp
	render/opengl_scene.hpp
	render/opengl_scene.cpp
	render/opengl_render_module.hpp
	render/opengl_render_module.cpp

	scene/map_ray_cast_result.hpp
	scene/map_generator.hpp
	scene/map_generator.cpp
	scene/map.hpp
	scene/map.cpp
	scene/world.hpp
	scene/world.cpp
	scene/scene.hpp
	scene/scene.cpp
	scene/scene_builder.hpp
	scene/scene_builder.cpp

	ui/base_imgui_element.hpp
	ui/imgui_text.hpp
	ui/imgui_text.cpp
	ui/imgui_button.hpp
	ui/imgui_button.cpp
	ui/imgui_check_box.hpp
	ui/imgui_check_box.cpp
	ui/imgui_input_text.hpp
	ui/imgui_input_text.cpp
	ui/imgui_list_box.hpp
	ui/imgui_list_box.cpp
	ui/imgui_window.hpp
	ui/imgui_window.cpp
)

add_library(BlocksCore SHARED ${CORE_SOURCE_FILES})
target_include_directories(BlocksCore
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(BlocksCore
	PUBLIC
		BlocksUtils
		OpenglObjectModel
		BlocksEnviroment
		BlocksModel
		
		FastNoise2::FastNoise
		EnTT::EnTT
)


target_include_directories(BlocksCore
	PRIVATE 
		"${PROJECT_BINARY_DIR}/configs"
)


install(
	IMPORTED_RUNTIME_ARTIFACTS
		glew
		glfw3
		FastNoise2::FastNoise

		BlocksCore
	DESTINATION
		.
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${CORE_SOURCE_FILES})
