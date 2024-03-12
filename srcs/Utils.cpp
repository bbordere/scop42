#include "Utils.hpp"

void split(std::string const &str, char delimiter,
		   std::vector<std::string> &vector) {

	std::string token;
	std::istringstream is(str);
	if (str.empty())
		return;
	while (std::getline(is, token, delimiter)) {
		std::size_t start = token.find_first_not_of(WHITE_SPACES);
		start = start != std::string::npos ? start : 0;
		std::size_t end = token.find_last_not_of(WHITE_SPACES);
		token = token.substr(start, end - start + 1);

		if (!token.empty())
			vector.emplace_back(token);
	}
}

float degToRad(float deg) {
	return ((deg * M_PI) / 180);
}

void resizeHandler(GLFWwindow *window, int w, int h) {
	void **pointers =
		reinterpret_cast<void **>(glfwGetWindowUserPointer(window));
	vec2u *vec = reinterpret_cast<vec2u *>(pointers[0]);
	*vec = {static_cast<unsigned int>(w), static_cast<unsigned int>(h)};
};

void dropHandler(GLFWwindow *window, int count, char const **paths) {
	static_cast<void>(count);
	void **pointers =
		reinterpret_cast<void **>(glfwGetWindowUserPointer(window));

	std::string path = paths[0];
	std::size_t dotPos = path.find_last_of('.');
	if (dotPos == std::string::npos)
		return;
	std::string extension = path.substr(dotPos);
	std::ptrdiff_t offset = -1;
	if (extension == ".bmp")
		offset = 2;
	else if (extension == ".obj")
		offset = 1;
	if (offset != -1) {
		std::string *valuePtr =
			reinterpret_cast<std::string *>(pointers[offset]);
		*valuePtr = path;
	}
}
