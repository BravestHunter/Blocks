#include "resource_base.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_INCLUDE_STB_IMAGE
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tiny_gltf.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

#include "io/file_api.hpp"


namespace blocks
{
  void ReadGltfNodeMeshes(const tinygltf::Model& gltfModel, const tinygltf::Node& gltfNode, const glm::mat4& transform, std::vector<Mesh>& meshes);
  void ReadGltfMesh(const tinygltf::Model& gltfModel, const tinygltf::Mesh& gltfMesh, const glm::mat4& transform, std::vector<Mesh>& meshes);
  std::vector<glm::vec2> GetGltfVec2DataVector(const tinygltf::Model& model, tinygltf::Accessor accessor);
  std::vector<glm::vec3> GetGltfVec3DataVector(const tinygltf::Model& model, tinygltf::Accessor accessor);
  std::vector<unsigned int> GetGltfUIntDataVector(const tinygltf::Model& model, tinygltf::Accessor accessor);
  Image GetGltfTexture(const tinygltf::Image& gltf_image, const tinygltf::Sampler& gltf_sampler);


  ResourceBase::ResourceBase()
  {
    blockSetPaths_ = std::make_shared<std::vector<std::string>>();

    //stbi_set_flip_vertically_on_load(true);
  }

  ResourceBase::~ResourceBase()
  {
  }


  void ResourceBase::SetUp(std::string path)
  {
    if (!blocks::isPathExist(path))
    {
      return;
    }

    size_t found = path.find_last_of("/\\");
    rootDirectory_ = (path.substr(0, found));

    std::string resourceBaseStr = blocks::readTextFile(path);

    nlohmann::json resourceBaseJson = nlohmann::json::parse(resourceBaseStr);
    nlohmann::json blockSets = resourceBaseJson["BlockSets"];
    for (auto& [key, value] : blockSets.items())
    {
      blockSetPaths_->push_back(value["Name"]);
    }
  }


  std::shared_ptr<std::vector<std::string>> ResourceBase::GetBlockSetNames()
  {
    return blockSetPaths_;
  }

  std::shared_ptr<BlockSet> ResourceBase::LoadBlockSet(std::string name)
  {
    std::string bsDirectory = rootDirectory_ + "\\" + "BlockSets" + "\\" + name;
    std::string bsFile = bsDirectory + "\\" + name + ".bs";

    std::string blockSetStr = blocks::readTextFile(bsFile);
    nlohmann::json resourceBaseJson = nlohmann::json::parse(blockSetStr);
    nlohmann::json blocksJson = resourceBaseJson["Blocks"];

    std::shared_ptr<BlockSet> blockSet = std::make_shared<BlockSet>(resourceBaseJson["Resolution"]);

    for (auto& [key, value] : blocksJson.items())
    {
      BlockInfo blockInfo;
      blockInfo.name = value["Name"];

      nlohmann::json blockTextures = value["Textures"];
      for (auto& [key, value] : blockTextures.items())
      {
        int index = std::stoi(key);
        blockInfo.textures[index] = value;
      }

      blockSet->AddBlockInfo(blockInfo);
    }

    int texturesNumber = resourceBaseJson["TexturesNumber"];
    for (int i = 0; i < texturesNumber; i++)
    {
      blockSet->AddTexture(bsDirectory + "\\" + std::to_string(i) + ".png");
    }

    return blockSet;
  }


  Image ResourceBase::ReadImage(std::string path)
  {
    Image result;

    std::vector<unsigned char> data = blocks::readBinaryFile(path);

    stbi_set_flip_vertically_on_load(true);

    unsigned char* rawData = stbi_load_from_memory(&data[0], data.size(), &result.width, &result.height, &result.channels, 0);
    result.data = std::vector<unsigned char>(rawData, rawData + result.height * result.width * result.channels);
    stbi_image_free(rawData);

    return result;
  }

  Model ResourceBase::ReadModel(std::string path)
  {
    std::string fileExtension = GetFileExtension(path);

    if (fileExtension != "gltf")
    {
      throw std::runtime_error("Model format is not supported");
    }

    return ReadGltfModel(path);
  }


  Model ResourceBase::ReadGltfModel(std::string path)
  {
    tinygltf::Model gltfModel;
    tinygltf::TinyGLTF gltfLoader;
    std::string error;
    std::string warning;

    gltfLoader.SetStoreOriginalJSONForExtrasAndExtensions(false);

    bool result = gltfLoader.LoadASCIIFromFile(&gltfModel, &error, &warning, path);
    if (result == false)
    {
      throw std::runtime_error("Failed to read glfw model");
    }
    
    std::vector<Mesh> meshes;
    for (const tinygltf::Scene& scene : gltfModel.scenes)
    {
      for (const int nodeIndex : scene.nodes)
      {
        const tinygltf::Node& gltfNode = gltfModel.nodes[nodeIndex];
        ReadGltfNodeMeshes(gltfModel, gltfNode, glm::mat4(1.0f), meshes);
      }
    }

    std::vector<Material> materials;
    for (size_t i = 0; i < gltfModel.materials.size(); i++)
    {
      const tinygltf::Material& gltfMaterial = gltfModel.materials[i];

      std::string name = gltfMaterial.name;

      if (gltfMaterial.pbrMetallicRoughness.baseColorTexture.index > -1)
      {
        const tinygltf::Texture& gltfTexture = gltfModel.textures[gltfMaterial.pbrMetallicRoughness.baseColorTexture.index];
        const tinygltf::Image& gltfImage = gltfModel.images[gltfTexture.source];
        const tinygltf::Sampler& gltfSampler = gltfTexture.sampler > -1 ? gltfModel.samplers[gltfTexture.sampler] : tinygltf::Sampler();

        Image texture = GetGltfTexture(gltfImage, gltfSampler);

        Material material(name, texture);
        materials.push_back(material);
      }
    }

    return Model(meshes, materials);
  }


  void ReadGltfNodeMeshes(const tinygltf::Model& gltfModel, const tinygltf::Node& gltfNode, const glm::mat4& transform, std::vector<Mesh>& meshes)
  {
    bool use_transform = gltfNode.matrix.size() > 0 || gltfNode.translation.size() > 0 || gltfNode.rotation.size() > 0 || gltfNode.scale.size() > 0;
    glm::mat4 localTransform = glm::mat4(1.0f);
    if (use_transform)
    {
      if (gltfNode.matrix.size() > 0)
      {
        float f_mat[16];
        for (int i = 0; i < 16; i++)
        {
          f_mat[i] = (float)gltfNode.matrix[i];
        }

        localTransform = glm::make_mat4(f_mat);
      }
      else
      {
        glm::mat4 translate = glm::mat4(1.0f);
        glm::mat4 rotate = glm::mat4(1.0f);
        glm::mat4 scale = glm::mat4(1.0f);

        if (gltfNode.translation.size() > 0)
        {
          translate = glm::translate(glm::mat4(1.0), glm::vec3((float)gltfNode.translation[0], (float)gltfNode.translation[1], (float)gltfNode.translation[2]));
        }

        if (gltfNode.rotation.size() > 0)
        {
          rotate = glm::toMat4(glm::quat((float)gltfNode.rotation[0], (float)gltfNode.rotation[1], (float)gltfNode.rotation[2], (float)gltfNode.rotation[3]));
        }

        if (gltfNode.scale.size() > 0)
        {
          scale = glm::scale(glm::mat4(1.0), glm::vec3((float)gltfNode.scale[0], (float)gltfNode.scale[1], (float)gltfNode.scale[2]));
        }

        localTransform = translate * rotate * scale;
      }
    }

    localTransform = transform * localTransform;
    //localTransform = glm::mat4(1.0f);

    if (gltfNode.mesh >= 0)
    {
      const tinygltf::Mesh& gltfMesh = gltfModel.meshes[gltfNode.mesh];
      ReadGltfMesh(gltfModel, gltfMesh, localTransform, meshes);
    }

    for (const int& childIndex : gltfNode.children)
    {
      const tinygltf::Node& childGltfNode = gltfModel.nodes[childIndex];
      ReadGltfNodeMeshes(gltfModel, childGltfNode, localTransform, meshes);
    }
  }

  void ReadGltfMesh(const tinygltf::Model& gltfModel, const tinygltf::Mesh& gltfMesh, const glm::mat4& transform, std::vector<Mesh>& meshes)
  {
    for (size_t j = 0; j < gltfMesh.primitives.size(); j++)
    {
      const tinygltf::Primitive gltfPrimitive = gltfMesh.primitives[j];

      bool hasTexcoords = gltfPrimitive.attributes.find("TEXCOORD_0") != gltfPrimitive.attributes.end();

      int material = gltfPrimitive.material;

      tinygltf::Accessor position_accessor = gltfModel.accessors[(*gltfPrimitive.attributes.find("POSITION")).second];
      std::vector<glm::vec3> positionData = GetGltfVec3DataVector(gltfModel, position_accessor);
      for (size_t k = 0; k < position_accessor.count; k++)
      {
        positionData[k] = glm::vec3(transform * glm::vec4(positionData[k], 1.0));

        // Flip x-z axes
        float temp = positionData[k].y;
        positionData[k].y = positionData[k].z;
        positionData[k].z = -temp;
      }

      std::vector<glm::vec2> texcoordsData;
      if (hasTexcoords)
      {
        texcoordsData = GetGltfVec2DataVector(gltfModel, gltfModel.accessors[(*gltfPrimitive.attributes.find("TEXCOORD_0")).second]);

        // Flip texture vertical coords
        for (size_t k = 0; k < texcoordsData.size(); k++)
        {
          texcoordsData[k].y = -texcoordsData[k].y;
        }
      }

      std::vector<unsigned int> indices;
      if (gltfPrimitive.indices > -1)
      {
        indices = GetGltfUIntDataVector(gltfModel, gltfModel.accessors[gltfPrimitive.indices]);
      }

      Mesh mesh(positionData, texcoordsData, indices, material);
      meshes.push_back(mesh);
    }
  }

  std::vector<glm::vec2> GetGltfVec2DataVector(const tinygltf::Model& model, tinygltf::Accessor accessor)
  {
    const tinygltf::BufferView bufferView = model.bufferViews[accessor.bufferView];
    const tinygltf::Buffer buffer = model.buffers[bufferView.buffer];

    int size = tinygltf::GetComponentSizeInBytes(accessor.componentType) * 2;
    std::vector<glm::vec2> result(accessor.count);

    if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_DOUBLE)
    {
      for (size_t i = 0; i < accessor.count; i++)
      {
        static double d[2];
        if (bufferView.byteStride > 0)
        {
          memcpy(&d, buffer.data.data() + bufferView.byteOffset + accessor.byteOffset + i * bufferView.byteStride, 16);
        }
        else
        {
          memcpy(&d, buffer.data.data() + bufferView.byteOffset + accessor.byteOffset + i * 16, 16);
        }

        result[i][0] = (float)d[0];
        result[i][1] = (float)d[1];
      }
    }
    else
    {
      if (bufferView.byteStride > 0)
      {
        for (size_t i = 0; i < accessor.count; i++)
        {
          memcpy(result.data() + i, buffer.data.data() + bufferView.byteOffset + accessor.byteOffset + i * bufferView.byteStride, size);
        }
      }
      else
      {
        memcpy(result.data(), buffer.data.data() + bufferView.byteOffset + accessor.byteOffset, accessor.count * size);
      }
    }

    return result;
  }

  std::vector<glm::vec3> GetGltfVec3DataVector(const tinygltf::Model& model, tinygltf::Accessor accessor)
  {
    const tinygltf::BufferView bufferView = model.bufferViews[accessor.bufferView];
    const tinygltf::Buffer buffer = model.buffers[bufferView.buffer];

    int size = tinygltf::GetComponentSizeInBytes(accessor.componentType) * 3;
    std::vector<glm::vec3> result(accessor.count);

    if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_DOUBLE)
    {
      for (size_t i = 0; i < accessor.count; i++)
      {
        static double d[3];
        if (bufferView.byteStride > 0)
        {
          memcpy(&d, buffer.data.data() + bufferView.byteOffset + accessor.byteOffset + i * bufferView.byteStride, 24);
        }
        else
        {
          memcpy(&d, buffer.data.data() + bufferView.byteOffset + accessor.byteOffset + i * 24, 24);
        }

        result[i][0] = (float)d[0];
        result[i][1] = (float)d[1];
        result[i][2] = (float)d[2];
      }
    }
    else // checked
    {
      if (bufferView.byteStride > 0)
      {
        for (size_t i = 0; i < accessor.count; i++)
        {
          memcpy(result.data() + i, buffer.data.data() + bufferView.byteOffset + accessor.byteOffset + i * bufferView.byteStride, size);
        }
      }
      else
      {
        memcpy(result.data(), buffer.data.data() + bufferView.byteOffset + accessor.byteOffset, accessor.count * size);
      }
    }

    return result;
  }


  std::vector<unsigned int> GetGltfUIntDataVector(const tinygltf::Model& model, tinygltf::Accessor accessor)
  {
    const tinygltf::BufferView bufferView = model.bufferViews[accessor.bufferView];
    const tinygltf::Buffer buffer = model.buffers[bufferView.buffer];

    std::vector<unsigned int> result(accessor.count);

    switch (accessor.componentType)
    {
    case TINYGLTF_COMPONENT_TYPE_BYTE:
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
      for (size_t i = 0; i < accessor.count; i++)
      {
        result[i] = (unsigned int)buffer.data[i + bufferView.byteOffset + accessor.byteOffset];
      }
      break;
    case TINYGLTF_COMPONENT_TYPE_SHORT:
      for (size_t i = 0; i < accessor.count; i++)
      {
        static short s;

        memcpy(&s, &buffer.data.at(bufferView.byteOffset + accessor.byteOffset + i * 2), 2);

        result[i] = (unsigned int)s;
      }
      break;
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
      for (size_t i = 0; i < accessor.count; i++)
      {
        static unsigned short us;

        memcpy(&us, &buffer.data.at(0) + bufferView.byteOffset + accessor.byteOffset + i * 2, 2);

        result[i] = (unsigned int)us;
      }
      break;
    default:
      memcpy(result.data(), buffer.data.data() + bufferView.byteOffset + accessor.byteOffset, accessor.count * tinygltf::GetComponentSizeInBytes(accessor.componentType));
      break;
    }

    return result;
  }

  Image GetGltfTexture(const tinygltf::Image& gltf_image, const tinygltf::Sampler& gltf_sampler)
  {
    Image texture;

    texture.data = std::vector<unsigned char>(gltf_image.image);
    texture.width = gltf_image.width;
    texture.height = gltf_image.height;
    texture.channels = gltf_image.component;

    return texture;
  }
}
