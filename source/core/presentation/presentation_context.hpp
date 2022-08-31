#pragma once

#include "render/opengl_scene.hpp"


namespace blocks
{
	struct PresentationContext
	{
		std::shared_ptr<OpenglScene> openglScene = nullptr;
		std::shared_ptr<BlockSet> blockSet = nullptr;
	};
}
