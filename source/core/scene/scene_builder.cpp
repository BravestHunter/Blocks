#include "scene_builder.hpp"

#include <format>

#include "io/file_api.hpp"
#include "ui/imgui_button.hpp"
#include "ui/imgui_text.hpp"


namespace blocks
{
  std::shared_ptr<Scene> SceneBuilder::BuildMainMenuScene(Game* game)
  {
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();

    std::shared_ptr<ImguiWindow> window = std::make_shared<ImguiWindow>("Main menu");
    scene->imguiWindows_.push_back(window);

    std::shared_ptr<ImguiButton> createWorldButton = std::make_shared<ImguiButton>(
      "Create new world",
      [game]()
      {
        srand(time(0));
        std::shared_ptr<Scene> worldScene_ = BuildWorldScene(game, std::make_shared<Map>(rand()));
        game->RequestScene(worldScene_);
      }
    );
    window->AddElement(createWorldButton);

    std::shared_ptr<ImguiButton> loadWorldButton = std::make_shared<ImguiButton>(
      "Load world",
      [game]()
      {
        if (!blocks::isPathExist("map"))
        {
          return;
        }

        std::shared_ptr<Map> map = Map::Load();
        std::shared_ptr<Scene> worldScene_ = BuildWorldScene(game, map);

        game->RequestScene(worldScene_);
      }
    );
    window->AddElement(loadWorldButton);

    return scene;
  }

  std::shared_ptr<Scene> SceneBuilder::BuildWorldScene(Game* game, std::shared_ptr<Map> map)
  {
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();

    scene->map_ = map;

    std::shared_ptr<ImguiWindow> window = std::make_shared<ImguiWindow>("Statistics");
    scene->imguiWindows_.push_back(window);

    std::shared_ptr<ImguiText> fpsText = std::make_shared<ImguiText>(
      []()
      {
        return std::format("Application average {0:.3f} ms/frame ({1:.0f} FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
      }
    );
    window->AddElement(fpsText);

    std::shared_ptr<ImguiText> cameraPositionText = std::make_shared<ImguiText>(
      [game]()
      {
        glm::vec3 position = game->GetContext().camera->GetPosition();
        return std::format("Camera position: {0:.2f} {1:.2f} {2:.2f}", position.x, position.y, position.z);
      }
    );
    window->AddElement(cameraPositionText);

    std::shared_ptr<ImguiText> cameraDirectionText = std::make_shared<ImguiText>(
      [game]()
      {
        glm::vec3 direction = game->GetContext().camera->GetForward();
        return  std::format("Camera direction: {0:.2f} {1:.2f} {2:.2f}", direction.x, direction.y, direction.z);
      }
    );
    window->AddElement(cameraDirectionText);

    std::shared_ptr<ImguiText> seedText = std::make_shared<ImguiText>(
      [game]()
      {
        return  std::format("Map seed: {}", game->GetContext().scene->GetMap()->GetSeed());
      }
    );
    window->AddElement(seedText);

    std::shared_ptr<ImguiButton> saveButton = std::make_shared<ImguiButton>(
      "Save world",
      [game]()
      {
        if (!blocks::isPathExist("map"))
        {
          blocks::createDirectory("map");
        }
        else
        {
          for (const std::string& path : blocks::getFilesInDirectory("map"))
          {
            blocks::removePath(path);
          }
        }

        Map::Save(game->GetContext().scene->GetMap());
      }
    );
    window->AddElement(saveButton);

    return scene;
  }
}
