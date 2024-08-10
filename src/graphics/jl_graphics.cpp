// Copyright (c) 2024 Jennie Scinocca
//-----------------------------------

#include "defines.h"
#include "graphics/jl_graphics.h"
#include "graphics/jl_graphics_vulkan.h"

#include <GLFW/glfw3.h>

#include <iostream>

using namespace std;

JlGraphics::JlGraphicsAPI activeAPI_;

bool JlGraphics::initGraphicsAPI(GLFWwindow* window, JlGraphicsAPI type) {
  activeAPI_ = type;

  switch (activeAPI_) {
    case JlGraphics::Vulkan:
      return JlVulkanGraphics::initVulkan(window);
      break;
    case JlGraphics::OpenGL:
      return false;
      break;
    case JlGraphics::DX12:
      return false;
      break;
    default:
      cout << JlEngineReports::jlInit << "Non-valid graphics API given."
        << endl;
      return false;
      break;
  }
}

void JlGraphics::shutdownGraphicsAPI() {
  switch (activeAPI_) {
    case JlGraphics::Vulkan:
      JlVulkanGraphics::shutdownVulkan();
      break;
    case JlGraphics::OpenGL:
      break;
    case JlGraphics::DX12:
      break;
    default:
      cout << JlEngineReports::jlInit << "Non-valid graphics API given."
        << endl;
      break;
  }
}