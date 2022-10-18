#pragma once

#include <memory>
#include <utility>
#include <unordered_map>

#include "entt/entity/entity.hpp"

#include "opengl_map.hpp"
#include "chunk.hpp"


namespace blocks
{
  class OpenglRenderSystem;

  class OpenglScene
  {
    friend OpenglRenderSystem;

  public:
    OpenglScene();
    ~OpenglScene();

    void InitMap();

    inline std::shared_ptr<OpenglMap> GetMap();
    inline std::unordered_map<entt::entity, AABB>& GetBounds();

  private:
    std::shared_ptr<OpenglMap> map_;
    std::unordered_map<entt::entity, AABB> bounds_;
  };


  std::shared_ptr<OpenglMap> OpenglScene::GetMap()
  {
    return map_;
  }

  std::unordered_map<entt::entity, AABB>& OpenglScene::GetBounds()
  {
    return bounds_;
  }
}
