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
		app.run();
	}
	catch (std::exception const &e) {
		std::cerr << e.what() << '\n';
		return (1);
	}

	// (void)ac;
	// (void)av;
	// BmpImage image;
	// image.extractData(av[1]);
	// File3D f;
	// f.load(av[1]);
}
