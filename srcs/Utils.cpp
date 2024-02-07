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

void split(std::string const &str, bool splitIndices,
		   std::vector<std::string> &vector) {

	std::string token;
	std::istringstream is(str);
	if (str.empty())
		return;
	while (std::getline(is, token, splitIndices ? '/' : ' ')) {
		token.erase(0, token.find_first_not_of("\t\n\v\f\r "));
		token.erase(token.find_last_not_of("\t\n\v\f\r ") + 1);
		if (!token.empty()) {
			vector.emplace_back(token);
		}
	}

	// std::regex regex(R"([\s]+)");
	// if (splitIndices)
	// 	regex = std::regex(R"([\s/]+)");

	// std::sregex_token_iterator it(str.begin(), str.end(), regex, -1);

	// std::sregex_token_iterator const end_tokens;
	// while (it != end_tokens)
	// 	vector.push_back(*it++);

	// vector.assign(words.begin(), words.end());

	// size_t start = 0;

	// for (size_t found = str.find(charset); found != std::string::npos;
	// 	 found = str.find(charset, start))
	// {
	// 	vector.emplace_back(str.begin() + start, str.begin() + found);
	// 	start = found + charset.size();
	// }
	// if (start != str.size())
	// 	vector.emplace_back(str.begin() + start, str.end());

	// std::string word;
	// std::size_t len = str.size();
	// for (std::size_t i = 0; i < len; ++i) {
	// 	if (charset.find(str[i]) != std::string::npos) {
	// 		if (!word.length())
	// 			continue;
	// 		vector.push_back(word);
	// 		word.clear();
	// 	}
	// 	else
	// 		word += str[i];
	// }
	// if (word.length())
	// 	vector.push_back(word);
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

	std::string *path = reinterpret_cast<std::string *>(pointers[1]);
	*path = paths[0];
}
