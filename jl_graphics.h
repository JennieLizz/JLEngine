// Copyright (c) 2024 Jennie Scinocca
//-----------------------------------

#pragma once
#include <GLFW/glfw3.h>

class JlGraphics
{
public:
  enum JlGraphicsAPI { Vulkan, OpenGL, DX12 };

  static bool initGraphicsAPI(GLFWwindow* window, JlGraphicsAPI type);
  static void shutdownGraphicsAPI();
};
