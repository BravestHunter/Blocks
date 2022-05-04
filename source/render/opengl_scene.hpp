#pragma once

#include <memory>
#include <utility>

#include "opengl_map.hpp"
#include "model/chunk.hpp"


class OpenglRenderSystem;

class OpenglScene
{
  friend OpenglRenderSystem;

public:
  OpenglScene();
  ~OpenglScene();

  void InitMap();
  void AddChunk(std::shared_ptr<Chunk> chunk, std::pair<int, int> position);
  void RemoveChunk(std::pair<int, int> position);

  std::shared_ptr<OpenglMap> GetMap();

private:
  std::shared_ptr<OpenglMap> map_;
};
