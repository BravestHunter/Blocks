#pragma once

#include "map_loading_context.hpp"
#include "render/opengl_scene.hpp"


namespace blocks
{
	struct PresentationContext
	{
		MapLoadingContext mapLoadingContext;
		std::shared_ptr<OpenglScene> openglScene = nullptr;
		std::shared_ptr<BlockSet> blockSet = nullptr;
	};
}
