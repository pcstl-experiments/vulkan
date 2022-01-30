CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
BINARIES = VulkanTest Instance BasicValidation

VulkanTest: 01-test.cpp
	g++ $(CFLAGS) -o VulkanTest 01-test.cpp $(LDFLAGS)

Instance: 02-instance.cpp
	g++ $(CFLAGS) -o Instance 02-instance.cpp $(LDFLAGS)

BasicValidation: 03-basic-validation.cpp
	g++ $(CFLAGS) -o BasicValidation 03-basic-validation.cpp $(LDFLAGS)

.PHONY: test-binary clean instance triangle

test-binary: VulkanTest
	./VulkanTest

instance: Instance
	./Instance

basic-validation: BasicValidation
	./BasicValidation

clean:
	rm -rf $(BINARIES)
