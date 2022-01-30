#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>

// #define DEBUG_EXTENSIONS
#ifndef NDEBUG
  #define ENABLE_VALIDATION_LAYERS
#endif

const std::vector<const char*> enabledValidationLayers = {
  "VK_LAYER_KHRONOS_validation"
};

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

class InstanceApplication {
public:
  void run() {
    initWindow();
    initVulkan();
    mainLoop();
    cleanUp();
  }

private:
  GLFWwindow* window;
  VkInstance instance;
  
  void initWindow() {
    glfwInit();
    
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
  }

  bool checkAllRequiredValidationLayersAvailable() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* requiredLayerName : enabledValidationLayers) {
      bool layerFound = false;

      for (const auto& layerProperties : availableLayers) {
	if (!strcmp(requiredLayerName, layerProperties.layerName)) {
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

  bool checkAllRequiredExtensionsAvailable(
    uint32_t requiredExtensionCount,
    const char** requiredExtensions,
    std::vector<VkExtensionProperties> availableExtensions
  ) {
    for (uint32_t i = 0; i < requiredExtensionCount; ++i) {
      bool foundExtension = false;
      const char* requiredExtensionName = requiredExtensions[i];

      for (uint32_t j = 0; j < availableExtensions.size(); ++j) {
	auto& availableExtensionProperties = availableExtensions[j];
	if (!strcmp(requiredExtensionName, availableExtensionProperties.extensionName)) {
	  foundExtension = true;
	  break;
	}
      }

      if (!foundExtension) {
	return false;
      }
    }

    return true;
  }

  const char** getRequiredExtensions(uint32_t* extensionCount) {
    const char** requiredExtensions = glfwGetRequiredInstanceExtensions(extensionCount);

    #ifdef DEBUG_EXTENSIONS
    std::cout << *extensionCount << " extensions required." << std::endl;
    std::cout << "Required Extensions:" << std::endl;
    for (uint32_t i = 0; i < *extensionCount; ++i) {
      std::cout << requiredExtensions[i] << std::endl;
    }
    std::cout << std::endl;
    #endif

    return requiredExtensions;
  }

  std::vector<VkExtensionProperties> getAvailableExtensions() {
    uint32_t availableExtensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(availableExtensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, availableExtensions.data());

    #ifdef DEBUG_EXTENSIONS
    std::cout << availableExtensionCount << " extensions available." << std::endl;
    std::cout << "Available Extensions:" << std::endl;
    for (const auto& extension : availableExtensions) {
      std::cout << extension.extensionName << std::endl;
    }
    std::cout << std::endl;
    #endif

    return availableExtensions;
  }

  VkApplicationInfo createAppInfo() {
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    return appInfo;
  }

  void createInstance() {
    VkApplicationInfo appInfo = createAppInfo();

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    uint32_t requiredExtensionCount = 0;
    const char** requiredExtensions = getRequiredExtensions(&requiredExtensionCount);
    
    createInfo.enabledExtensionCount = requiredExtensionCount;
    createInfo.ppEnabledExtensionNames = requiredExtensions;

    std::vector<VkExtensionProperties> availableExtensions = getAvailableExtensions();

    #ifdef ENABLE_VALIDATION_LAYERS
    createInfo.enabledLayerCount = static_cast<uint32_t>(enabledValidationLayers.size());
    createInfo.ppEnabledLayerNames = enabledValidationLayers.data();
    #else
    createInfo.enabledLayerCount = 0;
    #endif

    if (checkAllRequiredExtensionsAvailable(requiredExtensionCount, requiredExtensions, availableExtensions)) {
	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
	  throw std::runtime_error("Failed to create instance!");
	}
    } else {
	throw std::runtime_error("Missing some required extensions!");
    }
  }
  
  void initVulkan() {
    createInstance();
  }

  void mainLoop() {
    while(!glfwWindowShouldClose(window)) {
      glfwPollEvents();
    }
  }

  void cleanUp() {
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
  }
};

int main() {
  InstanceApplication app;

  try {
    app.run();
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
