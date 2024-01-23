#include "Utils.hpp"

std::vector<std::string> split(std::string str, std::string const &charset) {
	std::vector<std::string> res;
	std::string word;
	std::size_t len = str.size();
	for (std::size_t i = 0; i < len; ++i) {
		if (charset.find(str[i]) != std::string::npos) {
			if (!word.length())
				continue;
			res.push_back(word);
			word.clear();
		}
		else
			word += str[i];
	}
	if (word.length())
		res.push_back(word);
	return (res);
}

void split(std::string str, std::string const &charset,
		   std::vector<std::string> &vector) {
	std::string word;
	std::size_t len = str.size();
	for (std::size_t i = 0; i < len; ++i) {
		if (charset.find(str[i]) != std::string::npos) {
			if (!word.length())
				continue;
			vector.push_back(word);
			word.clear();
		}
		else
			word += str[i];
	}
	if (word.length())
		vector.push_back(word);
}

float degToRad(float deg) {
	return ((deg * M_PI) / 180);
}

void resizeHandler(GLFWwindow *window, int w, int h) {
	vec2u *ptr = reinterpret_cast<vec2u *>(glfwGetWindowUserPointer(window));
	*ptr = {static_cast<unsigned int>(w), static_cast<unsigned int>(h)};
};
