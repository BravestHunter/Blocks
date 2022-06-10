#include "map_loading_module.hpp"


namespace blocks
{
  MapLoadingModule::MapLoadingModule()
  {

  }

  MapLoadingModule::~MapLoadingModule()
  {

  }


  void MapLoadingModule::Update(float delta, GameContext& context)
  {
    if (context.scene->ContainsMap())
    {
      glm::ivec2 centerChunk = CalculateChunkCenter(context.camera->GetPosition());

      if (centerChunk != lastCenterChunkCoords_)
      {
        RemoveChunks(centerChunk, lastCenterChunkCoords_);
        AddChunks(centerChunk, context.scene->GetMap());

        lastCenterChunkCoords_ = centerChunk;
      }
    }
  }


  void MapLoadingModule::SetRenderModule(OpenglRenderModule* renderModule)
  {
    renderModule_ = renderModule;
  }


  void MapLoadingModule::OnSceneChanged(GameContext& context)
  {
    if (context.scene->ContainsMap())
    {
      lastCenterChunkCoords_ = CalculateChunkCenter(context.camera->GetPosition());
      AddChunks(lastCenterChunkCoords_, context.scene->GetMap());
    }
  }


  void MapLoadingModule::AddChunks(glm::ivec2 centerChunkCoords, std::shared_ptr<Map> map)
  {
    std::shared_ptr<OpenglMap> openglMap = renderModule_->GetOpenglScene()->GetMap();

    for (int x = centerChunkCoords.x - loadingRadius_; x <= centerChunkCoords.x + loadingRadius_; x++)
    {
      for (int y = centerChunkCoords.y - loadingRadius_; y <= centerChunkCoords.y + loadingRadius_; y++)
      {
        std::pair<int, int> coordinates = std::make_pair(x, y);

        if (!openglMap->ContainsChunk(coordinates))
        {
          std::shared_ptr<Chunk> chunk = map->GetChunk(coordinates);
          openglMap->EnqueueChunkAdd(chunk, coordinates);
        }
      }
    }
  }

  void MapLoadingModule::RemoveChunks(glm::ivec2 CenterChunkCoords, glm::ivec2 lastCenterChunkCoords)
  {
    std::shared_ptr<OpenglMap> openglMap = renderModule_->GetOpenglScene()->GetMap();
    glm::ivec2 xBorders = glm::ivec2(CenterChunkCoords.x - loadingRadius_, CenterChunkCoords.x + loadingRadius_);
    glm::ivec2 yBorders = glm::ivec2(CenterChunkCoords.y - loadingRadius_, CenterChunkCoords.y + loadingRadius_);

    for (int x = lastCenterChunkCoords.x - loadingRadius_; x <= lastCenterChunkCoords.x + loadingRadius_; x++)
    {
      for (int y = lastCenterChunkCoords.y - loadingRadius_; y <= lastCenterChunkCoords.y + loadingRadius_; y++)
      {
        if (x < xBorders.x || x > xBorders.y ||
          y < yBorders.x || y > yBorders.y)
        {
          std::pair<int, int> coordinates = std::make_pair(x, y);
          openglMap->EnqueueChunkRemove(coordinates);
        }
      }
    }
  }

  glm::ivec2 MapLoadingModule::CalculateChunkCenter(glm::vec3 position)
  {
    return glm::ivec2((int)position.x / Chunk::Length, (int)position.y / Chunk::Width);
  }
}
