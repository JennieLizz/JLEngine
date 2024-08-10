# Install script for directory: C:/Users/Jennie/source/repos/JLEngine/vcpkg_installed/x64-windows/vcpkg/blds/glfw3/src/3.4-2448ff4533.clean

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Users/Jennie/source/repos/JLEngine/vcpkg_installed/x64-windows/vcpkg/pkgs/glfw3_x64-windows/debug")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "OFF")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Jennie/source/repos/JLEngine/vcpkg_installed/x64-windows/vcpkg/blds/glfw3/x64-windows-dbg/src/cmake_install.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/Jennie/source/repos/JLEngine/vcpkg_installed/x64-windows/vcpkg/blds/glfw3/src/3.4-2448ff4533.clean/include/GLFW" FILES_MATCHING REGEX "/glfw3\\.h$" REGEX "/glfw3native\\.h$")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/glfw3" TYPE FILE FILES
    "C:/Users/Jennie/source/repos/JLEngine/vcpkg_installed/x64-windows/vcpkg/blds/glfw3/x64-windows-dbg/src/glfw3Config.cmake"
    "C:/Users/Jennie/source/repos/JLEngine/vcpkg_installed/x64-windows/vcpkg/blds/glfw3/x64-windows-dbg/src/glfw3ConfigVersion.cmake"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/glfw3/glfw3Targets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/glfw3/glfw3Targets.cmake"
         "C:/Users/Jennie/source/repos/JLEngine/vcpkg_installed/x64-windows/vcpkg/blds/glfw3/x64-windows-dbg/CMakeFiles/Export/f367bd07922f2ecfc14cf5547f1f7c4e/glfw3Targets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/glfw3/glfw3Targets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/glfw3/glfw3Targets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/glfw3" TYPE FILE FILES "C:/Users/Jennie/source/repos/JLEngine/vcpkg_installed/x64-windows/vcpkg/blds/glfw3/x64-windows-dbg/CMakeFiles/Export/f367bd07922f2ecfc14cf5547f1f7c4e/glfw3Targets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/glfw3" TYPE FILE FILES "C:/Users/Jennie/source/repos/JLEngine/vcpkg_installed/x64-windows/vcpkg/blds/glfw3/x64-windows-dbg/CMakeFiles/Export/f367bd07922f2ecfc14cf5547f1f7c4e/glfw3Targets-debug.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "C:/Users/Jennie/source/repos/JLEngine/vcpkg_installed/x64-windows/vcpkg/blds/glfw3/x64-windows-dbg/src/glfw3.pc")
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "C:/Users/Jennie/source/repos/JLEngine/vcpkg_installed/x64-windows/vcpkg/blds/glfw3/x64-windows-dbg/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
