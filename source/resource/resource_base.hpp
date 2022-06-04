#pragma once

#include <string>
#include <memory>
#include <vector>

#include "enviroment_system_interface.hpp"
#include "export.h"
#include "block_set.hpp"
#include "resource/image.hpp"


class DllExport ResourceBase : public EnviromentSystemInterface
{
public:
  ResourceBase();
  ResourceBase(const ResourceBase&) = delete;
  ResourceBase(ResourceBase&& other) = delete;
  ResourceBase& operator=(const ResourceBase&) = delete;
  ResourceBase& operator=(ResourceBase&& other) = delete;
  ~ResourceBase() override;

  void Init() override;
  void Deinit() override;
  bool IsInitialized() override;

  void SetUp(std::string path);

  std::shared_ptr<std::vector<std::string>> GetBlockSetNames();
  std::shared_ptr<BlockSet> LoadBlockSet(std::string name);

  Image ReadImage(std::string path);

private:
  std::string rootDirectory_;
  std::shared_ptr<std::vector<std::string>> blockSetPaths_;
};
