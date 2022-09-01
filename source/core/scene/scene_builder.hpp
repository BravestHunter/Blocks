#pragma once

#include <memory>

#include "game.hpp"
#include "scene.hpp"


namespace blocks
{
  class SceneBuilder
  {
  public:
    static std::shared_ptr<Scene> BuildMainMenuScene(Game* game);
    static std::shared_ptr<Scene> BuildWorldCreationScene(Game* game);
    static std::shared_ptr<Scene> BuildWorldLoadingScene(Game* game);
    static std::shared_ptr<Scene> BuildWorldScene(Game* game, std::shared_ptr<World> world);
  };
}
