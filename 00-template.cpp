#include <vulkan/vulkan.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>

class Application {
public:
  void run() {
    initVulkan();
    mainLoop();
    cleanUp();
  }

private:
  void initVulkan() {

  }

  void mainLoop() {

  }

  void cleanUp() {

  }
};

int main() {
  Application app;

  try {
    app.run();
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
