#pragma once

#include "Vectors.hpp"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

inline constexpr auto WHITE_SPACES = " \t\n\r\v\f";

template <typename T>
std::ostream &operator<<(std::ostream &stream, std::vector<T> const &vec) {
	if (!vec.size()) {
		stream << "[]";
		return (stream);
	}
	stream << '[';
	for (auto it = vec.cbegin(); it != vec.cend() - 1; ++it) {
		stream << *it << ", ";
	}
	stream << *(vec.cend() - 1) << ']';
	return (stream);
}

void split(std::string const &str, char delimiter,
		   std::vector<std::string> &vector);

float degToRad(float deg);
void resizeHandler(GLFWwindow *window, int w, int h);
void dropHandler(GLFWwindow *window, int count, char const **paths);
