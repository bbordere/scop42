#include "App.hpp"
// #include "BmpImage.hpp"
// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"

void test() {
	std::cout << "HELLO\n";
}

int main(int ac, char **av) {

	// KeyManager keyManager;

	// if (!glfwInit()) {
	// 	throw std::runtime_error("Cant init OpenGL !");
	// }

	// glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// GLFWwindow *window = glfwCreateWindow(800, 600, "scop", NULL, NULL);
	// keyManager.registerCallback(GLFW_KEY_SPACE,
	// 							std::function<void(void)>{&test}, PRESSED_ONCE);
	// while (!glfwWindowShouldClose(window)) {
	// 	/* code */
	// 	keyManager.updateKeysStates(window);
	// 	keyManager.executeActions();
	// 	glfwSwapBuffers(window);
	// 	glfwPollEvents();
	// }
	std::string path = "models/cube.obj";
	std::string texture = "res/nyan.bmp";
	if (ac == 2)
		path = av[1];
	if (ac == 3) {
		path = av[1];
		texture = av[2];
	}

	App app;
	try {
		app.init(path, texture);
	}
	catch (std::exception const &e) {
		std::cerr << e.what() << '\n';
		return (1);
	}
	app.run();

	// Model model;
	// model.load("models/Sword.obj");

	return (0);
}
