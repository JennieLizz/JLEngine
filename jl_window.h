// Copyright (c) 2024 Jennie Scinocca
//-----------------------------------

#pragma once
#include <GLFW/glfw3.h>

#include <cstdint>
#include <string>

using namespace std;

class JlWindow
{
public:
  JlWindow(uint32_t width, uint32_t height, string title);
  void destroyWindow();

  void startUpdates();
  GLFWwindow* getWindowPtr();

private:
  GLFWwindow* window_;
  uint32_t width_ = 800;
  uint32_t height_ = 600;
  string title_ = "JLE_APP";
};
