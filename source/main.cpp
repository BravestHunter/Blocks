#include <iostream>

#include "config.h"

#include "game.hpp"


int main()
{
  std::cout << "Hello, world!" << std::endl;
  std::cout << "Version " << Blocks_VERSION_MAJOR << "." << Blocks_VERSION_MINOR << std::endl;

  Game game;
  return game.Run();
}
