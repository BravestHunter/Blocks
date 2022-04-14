#include "game.hpp"

#include <iostream>

#define GLFW_INCLUDE_NONE
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "GLFW/glfw3.h"


Game::Game()
{

}

Game::~Game()
{

}


int Game::Run()
{
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "BlocksTitle", nullptr, nullptr);
  if (window == nullptr)
  {
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
  {
    return -1;
  }

  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);

  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return 0;
}
