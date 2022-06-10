#include "opengl_scene.hpp"

#include <exception>


namespace blocks
{
  OpenglScene::OpenglScene()
  {

  }

  OpenglScene::~OpenglScene()
  {

  }


  void OpenglScene::InitMap()
  {
    map_ = std::make_unique<OpenglMap>();
  }

  void OpenglScene::AddChunk(std::shared_ptr<Chunk> chunk, std::pair<int, int> position)
  {
    if (!map_)
    {
      throw std::exception("Map is not initialized");
    }

    map_->EnqueueChunkAdd(chunk, position);
  }

  void OpenglScene::RemoveChunk(std::pair<int, int> position)
  {
    if (!map_)
    {
      throw std::exception("Map is not initialized");
    }

    map_->EnqueueChunkRemove(position);
  }


  std::shared_ptr<OpenglMap> OpenglScene::GetMap()
  {
    return map_;
  }
}
