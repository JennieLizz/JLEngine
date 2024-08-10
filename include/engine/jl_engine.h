// Copyright (c) 2024 Jennie Scinocca
//-----------------------------------

#pragma once
#include "defines.h"

#include <filesystem>
#include <string>

using namespace std;
using namespace filesystem;

class JlEngine
{
public:
  JLEngine_API static void Init();
  void shutdown();
};

class JlEngineDirectories
{
public:
  JLEngine_API static void setEngineDirectory(const string& path);
  JLEngine_API static void setAppDirectory(const string& path);

  JLEngine_API static path engineDir;
  JLEngine_API static path appDir;
  JLEngine_API static path toolsDir;
  JLEngine_API static path shadersDir;
  JLEngine_API static path compiledShadersDir;
};