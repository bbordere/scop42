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

	// std::vector<std::string> vec;
	// std::string str = "Ceci est un test qwe 1/1/2";
	// split(str, '/', vec);
	// std::cout << vec << '\n';

	// BmpImage image;
	// image.extractData("res/lil.bmp");
	// (void)ac;

	// File3D file;
	// file.load(av[1]);
	// std::cout << file.getFaces().size() << '\n';
}
