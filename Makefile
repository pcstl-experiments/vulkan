CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
BINARIES = VulkanTest Instance

VulkanTest: 01-test.cpp
	g++ $(CFLAGS) -o VulkanTest 01-test.cpp $(LDFLAGS)

Instance: 02-triangle.cpp
	g++ $(CFLAGS) -o Instance 02-instance.cpp $(LDFLAGS)

.PHONY: test-binary clean instance

test-binary: VulkanTest
	./VulkanTest

instance: Instance
	./Instance

clean:
	rm -rf $(BINARIES)
