#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>

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

  bool checkAllRequiredExtensionsAvailable(
    uint32_t requiredExtensionCount,
    const char** requiredExtensions,
    std::vector<VkExtensionProperties> availableExtensions
  ) {
    for (uint32_t i = 0; i < requiredExtensionCount; ++i) {
      bool foundExtension = false;
      const char* requiredExtension = requiredExtensions[i];

      for (uint32_t j = 0; j < availableExtensions.size(); ++j) {
	auto& availableExtensionProperties = availableExtensions[j];
	if (!strcmp(requiredExtension, availableExtensionProperties.extensionName)) {
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

  void createInstance() {
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    std::cout << glfwExtensionCount << " extensions required." << std::endl;
    std::cout << "Required Extensions:" << std::endl;
    for (uint32_t i = 0; i < glfwExtensionCount; ++i) {
      std::cout << glfwExtensions[i] << std::endl;
    }
    std::cout << std::endl;

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    std::cout << extensionCount << " extensions available." << std::endl;
    std::cout << "Available Extensions:" << std::endl;
    for (const auto& extension : extensions) {
      std::cout << extension.extensionName << std::endl;
    }
    std::cout << std::endl;

    createInfo.enabledLayerCount = 0;

    if (checkAllRequiredExtensionsAvailable(glfwExtensionCount, glfwExtensions, extensions)) {
	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
	  throw std::runtime_error("Failed to create instance!");
	}
    } else {
	throw std::runtime_error("Missing some required extensions!");
    }

    std::cout << "Created instance successfully." << std::endl;
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
