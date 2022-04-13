#include <iostream>

//#define GLFW_INCLUDE_NONE
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "GLFW/glfw3.h"


#include "config.h"

const GLuint WIDTH = 800, HEIGHT = 600;

int showWindow()
{
  glfwInit();
  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
  if (window == nullptr)
  {
    glfwTerminate();
    return -1;
  }
  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

int main()
{
  std::cout << "Hello, world!" << std::endl;
  std::cout << "Version " << Blocks_VERSION_MAJOR << "." << Blocks_VERSION_MINOR << std::endl;

  return showWindow();
}
