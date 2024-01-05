#include "App.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// #include "BmpImage.hpp"
// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"

int main() {
	App app;
	try {
		app.init();
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
