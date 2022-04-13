#include <iostream>

#include "GLFW/glfw3.h"

#include "config.h"

int showWindow()
{
  if (!glfwInit())
  {
    return -1;
  }

  GLFWwindow* window = glfwCreateWindow(640, 480, "Title", NULL, NULL);
  if (!window)
  {
    return -1;
  }

  glfwDestroyWindow(window);

  glfwTerminate();

  return 0;
}

int main()
{
  std::cout << "Hello, world!" << std::endl;
  std::cout << "Version " << Blocks_VERSION_MAJOR << "." << Blocks_VERSION_MINOR << std::endl;

  return showWindow();
}
