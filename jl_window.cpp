// Copyright (c) 2024 Jennie Scinocca
//-----------------------------------

#include "defines.h"
#include "jl_window.h"

#include <GLFW/glfw3.h>

#include <cstdint>
#include <string>
#include <iostream>

using namespace std;

JlWindow::JlWindow(uint32_t width, uint32_t height, string title) {
  glfwInit();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  width_ = width;
  height_ = height;
  title_ = title;

  window_ =
    glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
}

void JlWindow::destroyWindow() {
  glfwDestroyWindow(window_);
  cout << JlEngineReports::jlWindow
    << "Window destroyed..." << endl;

  glfwTerminate();

  cout << JlEngineReports::jlWindow
    << "GLFW terminated..." << endl;

  cout << JlEngineReports::jlWindow
    << "Window was shut down successfully." << endl;
}

void JlWindow::startUpdates() {
  cout << JlEngineReports::jlWindow
    << "Window started updates." << endl;

  while (!glfwWindowShouldClose(window_)) {
    glfwPollEvents();
  }
}

GLFWwindow* JlWindow::getWindowPtr() { return window_; }
