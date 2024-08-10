// Copyright (c) 2024 Jennie Scinocca
//-----------------------------------

#pragma once
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>

#include <GLFW/glfw3.h>

#include <cstdint>
#include <optional>
#include <vector>

using namespace std;

class JlVulkanGraphics
{
public:
  static bool initVulkan(GLFWwindow* window);
  static void shutdownVulkan();

private:
  static bool createInstance();
  static bool setupDebugMessenger();
  static bool createSurface();
  static bool pickPhysicalDevice();
  static bool createLogicalDevice();
  static void createSwapChain();
  static bool createImageViews();
  static void createGraphicsPipeline();

  static vector<const char*> getRequiredExtensions();

  static bool isDeviceSuitable(VkPhysicalDevice device);
  static bool checkDeviceExtensionSupport(VkPhysicalDevice device);
  static int pickBestDevice(VkPhysicalDevice device);

  struct QueueFamilyIndices
  {
    optional<uint32_t> graphicsFamily;
    optional<uint32_t> presentFamily;

    bool isComplete() const {
      return graphicsFamily.has_value() && presentFamily.has_value();
    }
  };

  static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

  struct SwapChainSupportDetails
  {
    VkSurfaceCapabilitiesKHR capabilities{};
    vector<VkSurfaceFormatKHR> formats;
    vector<VkPresentModeKHR> presentModes;
  };

  static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

  static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const vector<VkSurfaceFormatKHR>& availableFormats);

  static VkPresentModeKHR chooseSwapPresentMode(const vector<VkPresentModeKHR>& availablePresentModes);

  static VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

  static bool checkValidationLayerSupport();

  static VKAPI_ATTR VkBool32 VKAPI_CALL
    debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                  VkDebugUtilsMessageTypeFlagsEXT messageType,
                  const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                  void* pUserData);

  static void populateDebugMessengerCreateInfo(
    VkDebugUtilsMessengerCreateInfoEXT& createInfo);

  static VkResult createDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pDebugMessenger);

  static void destroyDebugUtilsMessengerEXT(
    VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger_,
    const VkAllocationCallbacks* pAllocator);
};