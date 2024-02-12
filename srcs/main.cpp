#include "App.hpp"

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

	// BmpImage image;
	// image.extractData("res/lil.bmp");

	// File3D file;
	// file.load(av[1]);
}
