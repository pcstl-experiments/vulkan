CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
BINARIES = VulkanTest

VulkanTest: 01-test.cpp
	g++ $(CFLAGS) -o VulkanTest main.cpp $(LDFLAGS)

.PHONY: test-binary clean

test-binary: VulkanTest
	./VulkanTest

clean:
	rm -rf $(BINARIES)
