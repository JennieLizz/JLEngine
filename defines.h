// Copyright (c) 2024 Jennie Scinocca
//-----------------------------------

#pragma once
#ifdef JLEngine_EXPORTS
#define JLEngine_API __declspec(dllexport)
#else
#define JLEngine_API __declspec(dllimport)
#endif

struct JlEngineReports
{
  constexpr static const char* eTitle = "JLEngine";
  constexpr static const char* jlShader = "JLEngine/Shaders/ ";
  constexpr static const char* jlInit = "JLEngine/Init/ ";
  constexpr static const char* jlLaunch = "JLEngine/Launch/ ";
  constexpr static const char* jlGraphicsVulkan = "JLEngine/Graphics/Vulkan/ ";
  constexpr static const char* jlWindow = "JLEngine/Window/ ";
};