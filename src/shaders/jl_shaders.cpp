// Copyright (c) 2024 Jennie Scinocca
//-----------------------------------

#include "shaders/jl_shaders.h"

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

#include "defines.h"
#include "engine/jl_engine.h"

using namespace std;

bool JlVulkanShaders::compile() {
  system("cls");
  cout << ">> Directories ----" << endl;
  cout << "> " << JlEngineDirectories::toolsDir.string() << endl;
  cout << "> " << JlEngineDirectories::shadersDir.string() << endl;
  cout << "> " << JlEngineDirectories::engineDir.string() << endl;
  cout << "> " << JlEngineDirectories::appDir.string() << endl;
  cout << "> " << JlEngineDirectories::compiledShadersDir.string() << endl;
  cout << ">> ----------------" << endl;

  if (!exists(JlEngineDirectories::engineDir)) {
    cerr << JlEngineReports::jlShader
         << "Shaders can't compile. Failed to open the engine folder... The "
            "folder probably doesn't exist."
         << endl;
    return false;
  }

  if (!exists(JlEngineDirectories::toolsDir)) {
    cerr << JlEngineReports::jlShader
         << "Shaders can't compile. Failed to open the tools folder... The "
            "folder probably doesn't exist."
         << endl;
    return false;
  }

  if (!exists(JlEngineDirectories::compiledShadersDir)) {
    cerr << JlEngineReports::jlShader
         << "CSH folders doesn't exist, creating one now..." << endl;

    string cshDir = JlEngineDirectories::appDir.string() +
                    JlEngineDirectories::compiledShadersDir.string();
    create_directory(cshDir);
  }

  cout << JlEngineReports::jlShader << "Looking for shaders..." << endl;

  int shaderCompiledCount = 0;
  vector<directory_entry> shadersToCompile;
  vector<directory_entry> shadersToValidate;
  for (const directory_entry entry :
       directory_iterator(JlEngineDirectories::shadersDir)) {
    if (!entry.is_regular_file()) continue;

    string shName = entry.path().filename().string();
    path cshPath = JlEngineDirectories::appDir.string() +
                   JlEngineDirectories::compiledShadersDir.string() + shName +
                   ".spv";

    if (exists(cshPath)) {
      cout << " C > " << shName << endl;
      shadersToValidate.push_back(entry);
      shaderCompiledCount++;
      continue;
    }

    cout << " X > " << shName << endl;
    shadersToCompile.push_back(entry);
    shadersToValidate.push_back(entry);
  }

  cout << JlEngineReports::jlShader << "Completed." << endl;
  cout << JlEngineReports::jlShader << shadersToCompile.size()
       << " Shaders not compiled. / " << shaderCompiledCount
       << " Shaders already compiled." << endl;

  if (shadersToCompile.size() > 0) {
    int results = 0;
    cout << JlEngineReports::jlShader << "Compiling..." << endl;
    for (const directory_entry entry : shadersToCompile) {
      string shName = entry.path().filename().string();

      int result =
          system(("cd " + JlEngineDirectories::toolsDir.string()).c_str());
      if (result != 0) {
        cerr << JlEngineReports::jlShader
             << "Failed to open the tools folder... The folder probably "
                "doesn't exist."
             << endl;
        return false;
      }

      string glslangValidatorCmd =
          ("glslangValidator -V \"" + JlEngineDirectories::shadersDir.string() +
           shName + "\" -o \"" + JlEngineDirectories::appDir.string() +
           JlEngineDirectories::compiledShadersDir.string() + shName +
           ".spv\"");

      result = system(glslangValidatorCmd.c_str());
      if (result != 0) {
        cerr << JlEngineReports::jlShader << "Failed to compile \"" << shName
             << "\"." << endl;
        results++;
      }
    }

    if (results > 0)
      cout << JlEngineReports::jlShader << results
           << " Shaders failed to compile. Proceed with caution..." << endl;
    else
      cout << JlEngineReports::jlShader << "All shaders were compiled." << endl;
  } else
    cout << JlEngineReports::jlShader
         << "No shaders need to compile. ~If you need to force compile than "
            "start with \"-forceShaderCompile\"."
         << endl;

  if (shadersToValidate.size() > 0) {
    int results = 0;
    cout << JlEngineReports::jlShader << "Validating..." << endl;
    for (const directory_entry entry : shadersToValidate) {
      string shName = entry.path().filename().string();
      cout << " V > " + shName << ".spv" << endl;

      int result =
          system(("cd " + JlEngineDirectories::toolsDir.string()).c_str());
      if (result != 0) {
        cerr << JlEngineReports::jlShader
             << "Failed to open the tools folder... The folder probably "
                "doesn't exist."
             << endl;
        return false;
      }

      string glslangValidatorCmd =
          ("spirv-val --target-env vulkan1.3 \"" +
           JlEngineDirectories::appDir.string() +
           JlEngineDirectories::compiledShadersDir.string() + shName +
           ".spv\"");
      result = system(glslangValidatorCmd.c_str());
      if (result != 0) {
        cerr << JlEngineReports::jlShader << "Validation failed! \"" << shName
             << "\"." << endl;
        results++;
      }
    }

    if (results > 0)
      cout << JlEngineReports::jlShader << results
           << " Shaders failed to validate. Proceed with caution..." << endl;
    else
      cout << JlEngineReports::jlShader << "All shaders were validated."
           << endl;
  } else
    cout << JlEngineReports::jlShader << "No shaders to validate." << endl;

  return true;
}