#include "game.hpp"

#include <string>
#include <iostream>

#define GLFW_INCLUDE_NONE
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "GLFW/glfw3.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "shader.hpp"
#include "camera.hpp"


#define STRINGIZE_NX(A) #A
#define STRINGIZE(A) STRINGIZE_NX(A)
#define PPCAT_NX(A, B) A ## B
#define PPCAT(A, B) PPCAT_NX(A, B)

#include "resourceConfig.h"


// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


Game::Game(int width, int height) : framebufferWidth_(width), framebufferHeight_(height), lastX(width / 2), lastY(height / 2)
{
  platform_ = std::make_unique<GlfwPlatform>();
  camera_ = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
}

Game::~Game()
{
  
}


int Game::Run()
{
  platform_->Init();

  std::shared_ptr<GlfwWindow> window = platform_->CreateWindow(framebufferWidth_, framebufferHeight_, "Title");

  window->MakeCurrentContext();

  IMGUI_CHECKVERSION();

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
  {
    std::cout << "Failed to initialize GLEW" << std::endl;
    return -1;
  }
    
  glEnable(GL_DEPTH_TEST);

  Shader ourShader(PPCAT(SHADERS_DIR, DEFAULT_VERTEX_SHADER), PPCAT(SHADERS_DIR, DEFAULT_FRAGMENT_SHADER));

  float vertices[] = {
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
       0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
  };

  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);


  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char* data = stbi_load(PPCAT(TEXTURES_DIR, BLOCK_TEXTURE), &width, &height, &nrChannels, 0);
  if (data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  ourShader.Use();
  ourShader.SetInt("texture", 0);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  // Set callbacks
  window->SetFramebufferCallback(
    [this](int width, int height)
    {
      framebufferWidth_ = width;
      framebufferHeight_ = height;

      glViewport(0, 0, width, height);
    }
  );
  window->SetKeyCallback(
    [window](int keycode, int scancode, int action, int mods) 
    {
      if (action != GLFW_PRESS || window == nullptr)
        return;
  
      if (keycode == GLFW_KEY_ESCAPE)
      {
        window->SetWindowShouldClose(true);
      }
    }
  );
  window->SetCursorPositionCallback(
    [this](double xpos, double ypos)
    {
      float xposF = static_cast<float>(xpos);
      float yposF = static_cast<float>(ypos);

      if (firstMouse)
      {
        lastX = xposF;
        lastY = yposF;
        firstMouse = false;
      }

      float xoffset = xposF - lastX;
      float yoffset = lastY - yposF; // reversed since y-coordinates go from bottom to top

      lastX = xposF;
      lastY = yposF;

      camera_->ProcessMouseMovement(xoffset, yoffset);
    }
  );
  window->SetScrollCallback(
    [this](double xoffset, double yoffset)
    {
      camera_->ProcessMouseScroll(static_cast<float>(yoffset));
    }
  );


  while (!window->IsWindowShouldClose())
  {
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(window);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 projection = glm::perspective(glm::radians(camera_->GetZoom()), (float)framebufferWidth_ / (float)framebufferHeight_, 0.1f, 100.0f);
    ourShader.SetMat4("projection", projection);

    glm::mat4 view = camera_->GetViewMatrix();
    ourShader.SetMat4("view", view);

    glBindVertexArray(VAO);
    ourShader.SetMat4("model", glm::mat4(1.0f));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    window->SwapBuffers();
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

  platform_->Deinit();

  return 0;
}

void Game::processInput(std::shared_ptr<GlfwWindow> window)
{
  if (window->GetKeyState(GLFW_KEY_W) == GLFW_PRESS)
  {
    camera_->ProcessKeyboard(Camera::FORWARD, deltaTime);
  }
  if (window->GetKeyState(GLFW_KEY_S) == GLFW_PRESS)
  {
    camera_->ProcessKeyboard(Camera::BACKWARD, deltaTime);
  }
  if (window->GetKeyState(GLFW_KEY_A) == GLFW_PRESS)
  {
    camera_->ProcessKeyboard(Camera::LEFT, deltaTime);
  }
  if (window->GetKeyState(GLFW_KEY_D) == GLFW_PRESS)
  {
    camera_->ProcessKeyboard(Camera::RIGHT, deltaTime);
  }
}
