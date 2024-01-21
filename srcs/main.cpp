#include "App.hpp"
// #include "BmpImage.hpp"
// #define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
int main(int ac, char **av) {
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
}

// File3D model;
// model.load("models/Sword.obj");
