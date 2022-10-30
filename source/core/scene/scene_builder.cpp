#include "scene_builder.hpp"

#include <format>

#include "io/file_api.hpp"
#include "serialization.hpp"
#include "ui/imgui_button.hpp"
#include "ui/imgui_text.hpp"
#include "ui/imgui_check_box.hpp"
#include "ui/imgui_input_text.hpp"
#include "ui/imgui_list_box.hpp"


namespace blocks
{
  std::shared_ptr<Scene> SceneBuilder::BuildMainMenuScene(Game* game)
  {
    std::shared_ptr<Scene> scene = std::make_shared<Scene>(true);

    std::shared_ptr<ImguiWindow> menuWindow = std::make_shared<ImguiWindow>(glm::vec2(0.5f, 0.5f), glm::vec2(0.5f, 0.5f));
    scene->imguiWindows_.push_back(menuWindow);

    std::shared_ptr<ImguiButton> createWorldButton = std::make_shared<ImguiButton>(
      "Create new world",
      [game]()
      {
        std::shared_ptr<Scene> worldCreationScene_ = BuildWorldCreationScene(game);
        game->RequestScene(worldCreationScene_);
      }
    );
    menuWindow->AddElement(createWorldButton);

    std::shared_ptr<ImguiButton> loadWorldButton = std::make_shared<ImguiButton>(
      "Load world",
      [game]()
      {
        std::shared_ptr<Scene> worldLoadingScene_ = BuildWorldLoadingScene(game);
        game->RequestScene(worldLoadingScene_);
      }
    );
    menuWindow->AddElement(loadWorldButton);

    std::shared_ptr<ImguiButton> quitButton = std::make_shared<ImguiButton>(
      "Quit",
      [game]()
      {
        game->Stop();
      }
    );
    menuWindow->AddElement(quitButton);

    return scene;
  }

  std::shared_ptr<Scene> SceneBuilder::BuildWorldCreationScene(Game* game)
  {
    std::shared_ptr<Scene> scene = std::make_shared<Scene>(true);

    std::shared_ptr<ImguiWindow> menuWindow = std::make_shared<ImguiWindow>(glm::vec2(0.5f, 0.5f), glm::vec2(0.5f, 0.5f));
    scene->imguiWindows_.push_back(menuWindow);

    std::shared_ptr<ImguiInputText> worldNameInput = std::make_shared<ImguiInputText>(16, "World Name (16 characters max)");
    menuWindow->AddElement(worldNameInput);

    std::shared_ptr<ImguiButton> createWorldButton = std::make_shared<ImguiButton>(
      "Create",
      [game, worldNameInput]()
      {
        if (worldNameInput->GetText().length() == 0)
        {
          return;
        }

        srand(time(0));

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
    menuWindow->AddElement(createWorldButton);

    std::shared_ptr<ImguiButton> backButton = std::make_shared<ImguiButton>(
      "Back",
      [game]()
      {
        std::shared_ptr<Scene> mainMenuScene = BuildMainMenuScene(game);
        game->RequestScene(mainMenuScene);
      }
    );
    menuWindow->AddElement(backButton);

    return scene;
  }

  std::shared_ptr<Scene> SceneBuilder::BuildWorldLoadingScene(Game* game)
  {
    std::shared_ptr<Scene> scene = std::make_shared<Scene>(true);

    std::shared_ptr<ImguiWindow> menuWindow = std::make_shared<ImguiWindow>(glm::vec2(0.5f, 0.5f), glm::vec2(0.5f, 0.5f));
    scene->imguiWindows_.push_back(menuWindow);

    std::vector<std::string> existingWorlds;
    if (isPathExist("worlds"))
    {
      existingWorlds = getDirectoriesInDirectory("worlds");
    }
    std::shared_ptr<ImguiListBox> worldsListBox = std::make_shared<ImguiListBox>("Worlds", existingWorlds);
    menuWindow->AddElement(worldsListBox);

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
    menuWindow->AddElement(loadWorldButton);

    std::shared_ptr<ImguiButton> backButton = std::make_shared<ImguiButton>(
      "Back",
      [game]()
      {
        std::shared_ptr<Scene> mainMenuScene = BuildMainMenuScene(game);
        game->RequestScene(mainMenuScene);
      }
    );
    menuWindow->AddElement(backButton);

    return scene;
  }

  std::shared_ptr<Scene> SceneBuilder::BuildWorldScene(Game* game, std::shared_ptr<World> world)
  {
    std::shared_ptr<Scene> scene = std::make_shared<Scene>(false);

    scene->world_ = world;

    std::shared_ptr<ImguiWindow> menuWindow = std::make_shared<ImguiWindow>(glm::vec2(0.5f, 0.5f), glm::vec2(0.5f, 0.5f));
    scene->imguiWindows_.push_back(menuWindow);

    std::shared_ptr<ImguiButton> backButton = std::make_shared<ImguiButton>(
      "Back to Main Menu",
      [game]()
      {
        std::shared_ptr<Scene> mainMenuScene = BuildMainMenuScene(game);
        game->RequestScene(mainMenuScene);
      }
    );
    menuWindow->AddElement(backButton);

    std::shared_ptr<ImguiButton> quitButton = std::make_shared<ImguiButton>(
      "Quit",
      [game]()
      {
        game->Stop();
      }
    );
    menuWindow->AddElement(quitButton);

    std::shared_ptr<ImguiWindow> settingsWindow = std::make_shared<ImguiWindow>(glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 0.0f));
    scene->imguiWindows_.push_back(settingsWindow);

    std::shared_ptr<ImguiCheckBox> enableFlyModeCheckBox = std::make_shared<ImguiCheckBox>(
      game->GetContext().controlMode == ControlMode::Fly,
      "Fly mode",
      [game](bool checked)
      {
        game->GetContext().controlMode = checked ? ControlMode::Fly : ControlMode::Default;
      }
    );
    settingsWindow->AddElement(enableFlyModeCheckBox);

    std::shared_ptr<ImguiWindow> infoWindow = std::make_shared<ImguiWindow>("Info");
    scene->imguiWindows_.push_back(infoWindow);

    std::shared_ptr<ImguiText> worldNameText = std::make_shared<ImguiText>(std::format("Map {}", world->GetName()));
    infoWindow->AddElement(worldNameText);

    std::shared_ptr<ImguiText> mapSeedText = std::make_shared<ImguiText>(std::format("World seed: {}", world->GetMap()->GetSeed()));
    infoWindow->AddElement(mapSeedText);

    std::shared_ptr<ImguiText> fpsText = std::make_shared<ImguiText>(
      []()
      {
        return std::format("Application average {0:.3f} ms/frame ({1:.0f} FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
      }
    );
    infoWindow->AddElement(fpsText);

    std::shared_ptr<ImguiText> timeText = std::make_shared<ImguiText>(
      [game]()
      {
        return std::format("Scene time: {0:.3f} seconds", game->GetTimeState().GetSceneTime());
      }
    );
    infoWindow->AddElement(timeText);

    std::shared_ptr<ImguiText> cameraPositionText = std::make_shared<ImguiText>(
      [game]()
      {
        glm::vec3 position = game->GetContext().camera->GetPosition();
        return std::format("Camera position: {0:.2f} {1:.2f} {2:.2f}", position.x, position.y, position.z);
      }
    );
    infoWindow->AddElement(cameraPositionText);

    std::shared_ptr<ImguiText> cameraDirectionText = std::make_shared<ImguiText>(
      [game]()
      {
        glm::vec3 direction = game->GetContext().camera->GetForward();
        return  std::format("Camera direction: {0:.2f} {1:.2f} {2:.2f}", direction.x, direction.y, direction.z);
      }
    );
    infoWindow->AddElement(cameraDirectionText);

    std::shared_ptr<ImguiText> playerVelocityText = std::make_shared<ImguiText>(
      [game]()
      {
        glm::vec3 velocity = game->GetContext().scene->GetWorld()->GetEcsRegistry().get<PhysicsBody>(game->GetContext().scene->GetWorld()->GetPlayerEntity()).velocity;
        return  std::format("Player velocity: {0:.2f} {1:.2f} {2:.2f}", velocity.x, velocity.y, velocity.z);
      }
    );
    infoWindow->AddElement(playerVelocityText);

    std::shared_ptr<ImguiText> centerChunkText = std::make_shared<ImguiText>(
      [game]()
      {
        glm::vec3 position = game->GetContext().scene->GetWorld()->GetEcsRegistry().get<Transform>(game->GetContext().scene->GetWorld()->GetPlayerEntity()).position;
        ChunkPosition chunkPosition = Map::CalculateChunkPosition(position);
        return  std::format("Current chunk: {0} {1}", chunkPosition.x, chunkPosition.y);
      }
    );
    infoWindow->AddElement(centerChunkText);

    scene->menuModeCallbackFunction_ =
      [infoWindow, menuWindow](bool isMenuMode)
      {
        if (isMenuMode)
        {
          infoWindow->SetVisibility(false);
          menuWindow->SetVisibility(true);
        }
        else
        {
          infoWindow->SetVisibility(true);
          menuWindow->SetVisibility(false);
        }
      };

    return scene;
  }
}
