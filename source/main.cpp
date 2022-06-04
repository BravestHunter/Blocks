#include <iostream>

#include "config.h"
#include "resourceConfig.h"

#include "environment.hpp"
#include "game.hpp"

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;


void RunGame()
{
  Game game(SCR_WIDTH, SCR_HEIGHT);
  game.Run();
}

int main()
{
  std::cout << "Blocks application" << std::endl;
  std::cout << "Version " << Blocks_VERSION_MAJOR << "." << Blocks_VERSION_MINOR << std::endl;

  Enviroment::InitSystems();

  Enviroment::GetResourceSystem().SetUp(RESOURCE_BASE_PATH);

  RunGame();

  Enviroment::DeinitSystems();

  return 0;
}
