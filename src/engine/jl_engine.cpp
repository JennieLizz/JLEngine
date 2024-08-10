// Copyright (c) 2024 Jennie Scinocca
//-----------------------------------

#include "engine/jl_engine.h"
#include "graphics/jl_graphics.h"
#include "shaders/jl_shaders.h"
#include "engine/jl_window.h"

#include <GLFW/glfw3.h>

#include <filesystem>
#include <string>

JlEngine::JlEngine() {
  bool shaderCompile = JlVulkanShaders::compile();

  JlWindow* window = new JlWindow(1280, 720, "JLE_APP");
  bool result = JlGraphics::initGraphicsAPI(
    window->getWindowPtr(), JlGraphics::JlGraphicsAPI::Vulkan);


  if (!result) glfwSetWindowShouldClose(window->getWindowPtr(), GLFW_TRUE);
  window->startUpdates();

  JlGraphics::shutdownGraphicsAPI();
  window->destroyWindow();
  //shutdown();
}

void JlEngine::shutdown() {}

path JlEngineDirectories::engineDir = "";
path JlEngineDirectories::appDir = "";
path JlEngineDirectories::toolsDir = "";
path JlEngineDirectories::shadersDir = "";
path JlEngineDirectories::compiledShadersDir = "";

void JlEngineDirectories::setEngineDirectory(const string& path) {
  engineDir = path;
  toolsDir = path + "/tools/";
  shadersDir = path + "/shaders/";
}

void JlEngineDirectories::setAppDirectory(const string& path) {
  appDir = path + "/";
  compiledShadersDir = "csh/";
}
