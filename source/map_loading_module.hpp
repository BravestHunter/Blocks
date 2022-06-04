#pragma once

#include "glm/glm.hpp"

#include "game_module_interface.hpp"
#include "scene/map.hpp"
#include "render/opengl_map.hpp"
#include "render/opengl_render_module.hpp"


class MapLoadingModule : public GameModuleInterface
{
public:
  MapLoadingModule();
  MapLoadingModule(const MapLoadingModule&) = delete;
  MapLoadingModule(MapLoadingModule&& other) = delete;
  MapLoadingModule& operator=(const MapLoadingModule&) = delete;
  MapLoadingModule& operator=(MapLoadingModule&& other) = delete;
  ~MapLoadingModule() override;

  virtual void Update(float delta, GameContext& context) override;

  void SetRenderModule(OpenglRenderModule* renderModule);

  void OnSceneChanged(GameContext& context);

private:
  void AddChunks(glm::ivec2 centerChunkCoords, std::shared_ptr<Map> map);
  void RemoveChunks(glm::ivec2 centerChunkCoords, glm::ivec2 lastCenterChunkCoords);
  inline glm::ivec2 CalculateChunkCenter(glm::vec3 position);

  int loadingRadius_ = 3;
  glm::ivec2 lastCenterChunkCoords_;
  OpenglRenderModule* renderModule_ = nullptr;
};
