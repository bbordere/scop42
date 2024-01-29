#pragma once

#include "Vectors.hpp"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <sstream>
#include <string>
#include <vector>

// clang-format off

typedef struct s_color {
	float r;
	float g;
	float b;
} t_color;

template <typename T>
std::ostream &operator<<(std::ostream &stream, std::vector<T> const &set) {
	stream << '[';
	for (auto it = set.cbegin(); it != set.cend() - 1; it++) {
		stream << *it << ", ";
	}
	stream << *(set.cend() - 1) << ']';
	return (stream);
}

std::vector<std::string> split(std::string str, std::string const &charset);
void split(std::string str, std::string const &delimiter,
		   std::vector<std::string> &vector);

float degToRad(float deg);
void resizeHandler(GLFWwindow *window, int w, int h);
void dropHandler(GLFWwindow* window, int count, const char** paths);

// clang-format on
