#include "scene_builder.hpp"

#include <format>

#include "io/file_api.hpp"
#include "serialization.hpp"
#include "ui/imgui_button.hpp"
#include "ui/imgui_text.hpp"
#include "ui/imgui_input_text.hpp"
#include "ui/imgui_list_box.hpp"


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
        std::shared_ptr<Scene> worldCreationScene_ = BuildWorldCreationScene(game);
        game->RequestScene(worldCreationScene_);
      }
    );
    window->AddElement(createWorldButton);

    std::shared_ptr<ImguiButton> loadWorldButton = std::make_shared<ImguiButton>(
      "Load world",
      [game]()
      {
        std::shared_ptr<Scene> worldLoadingScene_ = BuildWorldLoadingScene(game);
        game->RequestScene(worldLoadingScene_);
      }
    );
    window->AddElement(loadWorldButton);

    return scene;
  }

  std::shared_ptr<Scene> SceneBuilder::BuildWorldCreationScene(Game* game)
  {
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();

    std::shared_ptr<ImguiWindow> window = std::make_shared<ImguiWindow>("World creation menu");
    scene->imguiWindows_.push_back(window);

    std::shared_ptr<ImguiInputText> worldNameInput = std::make_shared<ImguiInputText>(16, "World Name (16 characters max)");
    window->AddElement(worldNameInput);

    std::shared_ptr<ImguiButton> createWorldButton = std::make_shared<ImguiButton>(
      "Create",
      [game, worldNameInput]()
      {
        if (worldNameInput->GetText().length() == 0)
        {
          return;
        }

        WorldData worldData
        {
          .mapData = MapData
          {
            .seed = rand()
          },
          .playerData = PlayerData
          {
            .position = glm::vec3(8.0f, 8.0f, 270.0f)
          }
        };

        strcpy(worldData.name, worldNameInput->GetText().c_str());

        std::shared_ptr<Scene> worldScene_ = BuildWorldScene(game, std::make_shared<World>(worldData));
        game->RequestScene(worldScene_);
      }
    );
    window->AddElement(createWorldButton);

    std::shared_ptr<ImguiButton> backButton = std::make_shared<ImguiButton>(
      "Back",
      [game]()
      {
        std::shared_ptr<Scene> mainMenuScene = BuildMainMenuScene(game);
        game->RequestScene(mainMenuScene);
      }
    );
    window->AddElement(backButton);

    return scene;
  }

  std::shared_ptr<Scene> SceneBuilder::BuildWorldLoadingScene(Game* game)
  {
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();

    std::shared_ptr<ImguiWindow> window = std::make_shared<ImguiWindow>("World creation menu");
    scene->imguiWindows_.push_back(window);

    std::shared_ptr<ImguiListBox> worldsListBox = std::make_shared<ImguiListBox>("Worlds", getDirectoriesInDirectory("worlds"));
    window->AddElement(worldsListBox);

    std::shared_ptr<ImguiButton> loadWorldButton = std::make_shared<ImguiButton>(
      "Load",
      [game, worldsListBox]()
      {
        std::string selectedWorldName = worldsListBox->GetSelectedItem();

        std::string worldDataPath = std::format("worlds/{0}/world.dat", selectedWorldName);
        if (isPathExist(worldDataPath) == false)
        {
          return;
        }

        std::vector<unsigned char> bytes = readBinaryFile(worldDataPath);
        WorldData worldData = deserializeWorld(bytes);

        std::shared_ptr<Scene> worldScene_ = BuildWorldScene(game, std::make_shared<World>(worldData));
        game->RequestScene(worldScene_);
      }
    );
    window->AddElement(loadWorldButton);

    std::shared_ptr<ImguiButton> backButton = std::make_shared<ImguiButton>(
      "Back",
      [game]()
      {
        std::shared_ptr<Scene> mainMenuScene = BuildMainMenuScene(game);
        game->RequestScene(mainMenuScene);
      }
    );
    window->AddElement(backButton);

    return scene;
  }

  std::shared_ptr<Scene> SceneBuilder::BuildWorldScene(Game* game, std::shared_ptr<World> world)
  {
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();

    scene->world_ = world;

    std::shared_ptr<ImguiWindow> window = std::make_shared<ImguiWindow>("Statistics");
    scene->imguiWindows_.push_back(window);

    std::shared_ptr<ImguiText> lockCursorHelpText = std::make_shared<ImguiText>("(Press \"L\" to lock/unlock cursor)");
    window->AddElement(lockCursorHelpText);

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

    const int seed = world->GetMap()->GetSeed();
    std::shared_ptr<ImguiText> seedText = std::make_shared<ImguiText>(
      [seed]()
      {
        return  std::format("Map seed: {}", seed);
      }
    );
    window->AddElement(seedText);

    return scene;
  }
}
