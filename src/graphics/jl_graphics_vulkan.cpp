// Copyright (c) 2024 Jennie Scinocca
//-----------------------------------

#include "graphics/jl_graphics_vulkan.h"

#include <GLFW/glfw3.h>
#include <string.h>
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>

#include <cstdint>
#include <iostream>
#include "defines.h"
#include <algorithm>
#include <limits>
#include <set>
#include <string>
#include <vector>
#include <stdexcept>

using namespace std;

#ifdef NDEBUG
const bool enableValidationLayers_ = false;
#else
const bool enableValidationLayers_ = true;
#endif

const vector<const char*> validationLayers_ = { "VK_LAYER_KHRONOS_validation" };
const vector<const char*> deviceExtensions_ = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

GLFWwindow* window_;

VkInstance instance_;
VkDebugUtilsMessengerEXT debugMessenger_;
VkPhysicalDevice physicalDevice_ = VK_NULL_HANDLE;
VkPhysicalDeviceProperties deviceProperties_;
VkPhysicalDeviceFeatures deviceFeatures_;
VkDevice device_;

VkSurfaceKHR surface_;

VkQueue graphicsQueue_;
VkQueue presentQueue_;

VkSwapchainKHR swapChain_;
vector<VkImage> swapChainImages_;
VkFormat swapChainImageFormat_;
VkExtent2D swapChainExtent_;

vector<VkImageView> swapChainImageViews_;

bool JlVulkanGraphics::initVulkan(GLFWwindow* window) {
  cout << JlEngineReports::jlGraphicsVulkan
    << "Initializing Graphics API..." << endl;

  window_ = window;

  if (!createInstance()) return false;
  setupDebugMessenger();
  if (!createSurface()) return false;
  if (!pickPhysicalDevice()) return false;
  if (!createLogicalDevice()) return false;
  createSwapChain();
  if (!createImageViews()) return false;
  return true;
}

void JlVulkanGraphics::shutdownVulkan() {
  cout << JlEngineReports::jlGraphicsVulkan
    << "Shutting down Vulkan Graphics API..." << endl;

  for (VkImageView imageView : swapChainImageViews_)
    vkDestroyImageView(device_, imageView, nullptr);

  cout << JlEngineReports::jlGraphicsVulkan
    << "Image views destroyed..." << endl;

  vkDestroySwapchainKHR(device_, swapChain_, nullptr);

  cout << JlEngineReports::jlGraphicsVulkan
    << "Swap chain destroyed..." << endl;

  vkDestroyDevice(device_, nullptr);

  cout << JlEngineReports::jlGraphicsVulkan
    << "Device destroyed..." << endl;

  if (enableValidationLayers_) {
    destroyDebugUtilsMessengerEXT(instance_, debugMessenger_, nullptr);
    cout << JlEngineReports::jlGraphicsVulkan
      << "Debug messenger destroyed..." << endl;
  }

  vkDestroySurfaceKHR(instance_, surface_, nullptr);

  cout << JlEngineReports::jlGraphicsVulkan
    << "Surface destroyed..." << endl;

  vkDestroyInstance(instance_, nullptr);

  cout << JlEngineReports::jlGraphicsVulkan
    << "Instance destroyed..." << endl;

  cout << JlEngineReports::jlGraphicsVulkan
    << "Vulkan was shut down successfully." << endl;
}

bool JlVulkanGraphics::createInstance() {
  try {
    if (enableValidationLayers_) {
      if (!checkValidationLayerSupport())
        throw runtime_error("No support for Validation Layers");
    } else {
      throw runtime_error("Validation layers requested, but not available.");
    }
  }
  catch (const runtime_error& e) {
    cerr << JlEngineReports::jlGraphicsVulkan << e.what() << endl;
  }

  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "JLE_APP";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "JLE";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_3;

  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  vector<const char*> extensions = getRequiredExtensions();
  createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  createInfo.ppEnabledExtensionNames = extensions.data();

  VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
  if (enableValidationLayers_) {
    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers_.size());
    createInfo.ppEnabledLayerNames = validationLayers_.data();

    populateDebugMessengerCreateInfo(debugCreateInfo);
    createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
  }
  else {
    createInfo.enabledLayerCount = 0;

    createInfo.pNext = nullptr;
  }

  try {
    if (vkCreateInstance(&createInfo, nullptr, &instance_) != VK_SUCCESS)
      throw runtime_error("Failed to create a instance.");
  }
  catch (const runtime_error& e) {
    cerr << JlEngineReports::jlGraphicsVulkan << e.what() << endl;
    return false;
  }

  cout << JlEngineReports::jlGraphicsVulkan << "Instance created..."
    << endl;
  return true;
}

bool JlVulkanGraphics::setupDebugMessenger() {
  if (!enableValidationLayers_) return true;

  VkDebugUtilsMessengerCreateInfoEXT createInfo;
  populateDebugMessengerCreateInfo(createInfo);

  try {
    if (createDebugUtilsMessengerEXT(instance_, &createInfo, nullptr, &debugMessenger_) != VK_SUCCESS)
      throw runtime_error("Failed to set up debug messenger.");
  }
  catch (const runtime_error& e) {
    cerr << JlEngineReports::jlGraphicsVulkan << e.what() << endl;
    return false;
  }

  return true;
}

bool JlVulkanGraphics::createSurface() {
  try {
    if (glfwCreateWindowSurface(instance_, window_, nullptr, &surface_) != VK_SUCCESS)
      throw runtime_error("Failed to create surface.");
  }
  catch (const runtime_error& e) {
    cerr << JlEngineReports::jlGraphicsVulkan << e.what() << endl;
    return false;
  }

  cout << JlEngineReports::jlGraphicsVulkan << "Surface created..."
    << endl;
  return true;
}

bool JlVulkanGraphics::pickPhysicalDevice() {
  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(instance_, &deviceCount, nullptr);

  try {
    if (deviceCount == 0)
      throw runtime_error("Failed to find GPUs with Vulkan support.");
  }
  catch (const runtime_error& e) {
    cerr << JlEngineReports::jlGraphicsVulkan << e.what() << endl;
  }

  vector<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(instance_, &deviceCount, devices.data());

  /*multimap<int, VkPhysicalDevice> candidates;
  for (const VkPhysicalDevice& device : devices) {
    int score = pickBestDevice(device);
    cout << score << endl;
    candidates.insert(make_pair(score, device));
  }*/

  for (const VkPhysicalDevice& device : devices) {
    if (isDeviceSuitable(device)) {
      physicalDevice_ = device;
      break;
    }
  }

  try {
    /*if (candidates.rbegin()->first > 0)
      physicalDevice_ = candidates.rbegin()->second;
    else
      throw runtime_error("Failed to find a suitable GPU.");*/

    if (physicalDevice_ == VK_NULL_HANDLE)
      throw runtime_error("Failed to find a suitable GPU.");
  }
  catch (const runtime_error& e) {
    cerr << JlEngineReports::jlGraphicsVulkan << e.what() << endl;
    return false;
  }

  cout << JlEngineReports::jlGraphicsVulkan << "Physical device picked..."
    << endl;
  return true;
}

bool JlVulkanGraphics::createLogicalDevice() {
  QueueFamilyIndices indices = findQueueFamilies(physicalDevice_);

  vector<VkDeviceQueueCreateInfo> queueCreateInfos;
  set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(),
                                       indices.presentFamily.value() };

  float queuePriority = 1.0f;
  for (uint32_t queueFamily : uniqueQueueFamilies) {
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = queueFamily;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;
    queueCreateInfos.push_back(queueCreateInfo);
  }

  VkPhysicalDeviceFeatures deviceFeatures{};

  VkDeviceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

  createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
  createInfo.pQueueCreateInfos = queueCreateInfos.data();

  createInfo.pEnabledFeatures = &deviceFeatures;

  createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions_.size());
  createInfo.ppEnabledExtensionNames = deviceExtensions_.data();

  if (enableValidationLayers_) {
    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers_.size());
    createInfo.ppEnabledLayerNames = validationLayers_.data();
  }
  else {
    createInfo.enabledLayerCount = 0;
  }

  try {
    if (vkCreateDevice(physicalDevice_, &createInfo, nullptr, &device_) !=
        VK_SUCCESS)
      throw runtime_error("Failed to create logical device.");
  }
  catch (runtime_error& e) {
    cerr << JlEngineReports::jlGraphicsVulkan << e.what() << endl;
    return false;
  }

  vkGetDeviceQueue(device_, indices.graphicsFamily.value(), 0, &graphicsQueue_);
  vkGetDeviceQueue(device_, indices.presentFamily.value(), 0, &presentQueue_);

  cout << JlEngineReports::jlGraphicsVulkan << "Logical device created..."
    << endl;
  return true;
}

void JlVulkanGraphics::createSwapChain() {
  SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice_);

  VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
  VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
  VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

  uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
  if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
    imageCount = swapChainSupport.capabilities.maxImageCount;
  }

  VkSwapchainCreateInfoKHR createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface = surface_;

  createInfo.minImageCount = imageCount;
  createInfo.imageFormat = surfaceFormat.format;
  createInfo.imageColorSpace = surfaceFormat.colorSpace;
  createInfo.imageExtent = extent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  QueueFamilyIndices indices = findQueueFamilies(physicalDevice_);
  uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

  if (indices.graphicsFamily != indices.presentFamily) {
    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices = queueFamilyIndices;
  }
  else {
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  }

  createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  createInfo.presentMode = presentMode;
  createInfo.clipped = VK_TRUE;

  createInfo.oldSwapchain = VK_NULL_HANDLE;

  try {
    if (vkCreateSwapchainKHR(device_, &createInfo, nullptr, &swapChain_) != VK_SUCCESS)
      throw runtime_error("Failed to create swap chain.");
  }
  catch (runtime_error& e) {
    cerr << JlEngineReports::jlGraphicsVulkan << e.what() << endl;
  }

  vkGetSwapchainImagesKHR(device_, swapChain_, &imageCount, nullptr);
  swapChainImages_.resize(imageCount);
  vkGetSwapchainImagesKHR(device_, swapChain_, &imageCount, swapChainImages_.data());

  swapChainImageFormat_ = surfaceFormat.format;
  swapChainExtent_ = extent;

  cout << JlEngineReports::jlGraphicsVulkan << "Swap chain created..."
    << endl;
}

bool JlVulkanGraphics::createImageViews() {
  swapChainImageViews_.resize(swapChainImages_.size());

  for (size_t i = 0; i < swapChainImages_.size(); i++) {
    VkImageViewCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = swapChainImages_[i];
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = swapChainImageFormat_;
    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;

    try {
      if (vkCreateImageView(device_, &createInfo, nullptr, &swapChainImageViews_[i]) != VK_SUCCESS)
        throw std::runtime_error("Failed to create image views.");
    }
    catch (runtime_error& e) {
      cerr << JlEngineReports::jlGraphicsVulkan << e.what() << endl;
      return false;
    }
  }

  cout << JlEngineReports::jlGraphicsVulkan << "Image views created..."
    << endl;
  return true;
}

void JlVulkanGraphics::createGraphicsPipeline() {
}

bool JlVulkanGraphics::isDeviceSuitable(VkPhysicalDevice device) {
  QueueFamilyIndices indices = findQueueFamilies(device);

  bool extensionsSupported = checkDeviceExtensionSupport(device);

  bool swapChainAdequate = false;
  if (extensionsSupported) {
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
    swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
  }

  cout << JlEngineReports::jlGraphicsVulkan << "Device confirmed suitable..."
    << endl;
  return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

vector<const char*> JlVulkanGraphics::getRequiredExtensions() {
  uint32_t glfwExtensionCount = 0;
  const char** glfwExtensions;
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  vector<const char*> extensions(glfwExtensions,
                                 glfwExtensions + glfwExtensionCount);

  if (enableValidationLayers_) {
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

  cout << JlEngineReports::jlGraphicsVulkan << "Found required extensions..."
    << endl;
  return extensions;
}

bool JlVulkanGraphics::checkDeviceExtensionSupport(VkPhysicalDevice device) {
  uint32_t extensionCount;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                       nullptr);

  vector<VkExtensionProperties> availableExtensions(extensionCount);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                       availableExtensions.data());

  set<string> requiredExtensions(deviceExtensions_.begin(),
                                 deviceExtensions_.end());

  for (const auto& extension : availableExtensions) {
    requiredExtensions.erase(extension.extensionName);
  }

  return requiredExtensions.empty();
}

int JlVulkanGraphics::pickBestDevice(VkPhysicalDevice device) {
  int score = 0;

  if (deviceProperties_.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
    score += 1000;

  score += deviceProperties_.limits.maxImageDimension2D;

  if (!deviceFeatures_.geometryShader) {
    cerr << JlEngineReports::jlGraphicsVulkan
      << "GPU doesn't support Geometry Shaders." << endl;
    return 0;
  }

  return score;
}

JlVulkanGraphics::QueueFamilyIndices JlVulkanGraphics::findQueueFamilies(
  VkPhysicalDevice device) {
  QueueFamilyIndices indices{};

  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

  vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
                                           queueFamilies.data());

  int i = 0;
  for (const auto& queueFamily : queueFamilies) {
    if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
      indices.graphicsFamily = i;

    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface_, &presentSupport);
    if (presentSupport) indices.presentFamily = i;

    if (indices.isComplete()) break;

    i++;
  }

  return indices;
}

JlVulkanGraphics::SwapChainSupportDetails JlVulkanGraphics::querySwapChainSupport(VkPhysicalDevice device) {
  SwapChainSupportDetails details;

  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface_, &details.capabilities);

  uint32_t formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface_, &formatCount, nullptr);

  if (formatCount != 0) {
    details.formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface_, &formatCount, details.formats.data());
  }

  uint32_t presentModeCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface_, &presentModeCount, nullptr);

  if (presentModeCount != 0) {
    details.presentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface_, &presentModeCount, details.presentModes.data());
  }

  return details;
}

VkSurfaceFormatKHR JlVulkanGraphics::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
  for (const auto& availableFormat : availableFormats) {
    if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return availableFormat;
    }
  }

  return availableFormats[0];
}

VkPresentModeKHR JlVulkanGraphics::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
  for (const auto& availablePresentMode : availablePresentModes) {
    if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
      return availablePresentMode;
    }
  }

  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D JlVulkanGraphics::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
  if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
    return capabilities.currentExtent;
  }
  else {
    int width, height;
    glfwGetFramebufferSize(window_, &width, &height);

    VkExtent2D actualExtent = {
        static_cast<uint32_t>(width),
        static_cast<uint32_t>(height)
    };

    actualExtent.width = clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    actualExtent.height = clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

    return actualExtent;
  }
}

bool JlVulkanGraphics::checkValidationLayerSupport() {
  uint32_t layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

  vector<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  for (const char* layerName : validationLayers_) {
    bool layerFound = false;

    for (const auto& layerProperties : availableLayers) {
      if (strcmp(layerName, layerProperties.layerName) == 0) {
        layerFound = true;
        break;
      }
    }

    if (!layerFound) {
      return false;
    }
  }

  return true;
}

VKAPI_ATTR VkBool32 VKAPI_CALL JlVulkanGraphics::debugCallback(
  VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
  VkDebugUtilsMessageTypeFlagsEXT messageType,
  const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
  void* pUserData) {
  cout << JlEngineReports::jlGraphicsVulkan
    << "Validation layer: " << pCallbackData->pMessage << endl;

  return VK_FALSE;
}

void JlVulkanGraphics::populateDebugMessengerCreateInfo(
  VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
  createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  createInfo.pfnUserCallback = debugCallback;
}

VkResult JlVulkanGraphics::createDebugUtilsMessengerEXT(
  VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
  const VkAllocationCallbacks* pAllocator,
  VkDebugUtilsMessengerEXT* pDebugMessenger) {
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
    instance, "vkCreateDebugUtilsMessengerEXT");
  if (func != nullptr) {
    return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
  }
  else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

void JlVulkanGraphics::destroyDebugUtilsMessengerEXT(
  VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger_,
  const VkAllocationCallbacks* pAllocator) {
  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
    instance, "vkDestroyDebugUtilsMessengerEXT");
  if (func != nullptr) {
    func(instance, debugMessenger_, pAllocator);
  }
}