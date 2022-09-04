#pragma once

#include <memory>

#include "collections/concurrent_queue.hpp"
#include "simulation/camera.hpp"
#include "scene/scene.hpp"
#include "simulation/base_model_update_event.hpp"
#include "geometry/aabb.hpp"
#include "render/opengl_scene.hpp"


namespace blocks
{
  struct GameContext
  {
    std::shared_ptr<Scene> scene;
    ConcurrentQueue<std::shared_ptr<BaseModelUpdateEvent>> modelUpdateEventsQueue;
    std::shared_ptr<Camera> camera;

    bool isMenuMode = true;
    bool isGravityEnabled = false;
  };
}
