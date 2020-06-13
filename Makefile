CFLAGS = -std=c++17
LDFLAGS = `pkg-config --static --libs glfw3` -lvulkan

vk_test: main.cpp
	g++ $(CFLAGS) -o vk_test main.cpp $(LDFLAGS)

.PHONY: test clean

test: vk_test
	VK_LAYER_PATH=/usr/share/vulkan/explicit_layer.d ./vk_test

clean:
	rm -f vk_test
