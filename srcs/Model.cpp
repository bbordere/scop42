#include "Model.hpp"

#include <iostream>
void Model::load(std::string const &path) {
	std::srand(std::time(nullptr));
	std::regex obj_regex(".+(\\.obj)$");
	if (!std::regex_match(path, obj_regex))
		throw std::runtime_error("Bad file format !");
	std::ifstream file;
	file.open(path);
	if (!file.is_open())
		throw std::runtime_error("Can't open file !");
	std::string line;
	std::vector<std::string> input;
	while (std::getline(file, line)) {
		split(line, " ", input);
		if (input[0] == "v")
			this->vertices.push_back({std::stof(input[1]), std::stof(input[2]),
									  std::stof(input[3])});
		else if (input[0] == "vn")
			this->normals.push_back({std::stof(input[1]), std::stof(input[2]),
									 std::stof(input[3])});
		else if (input[0] == "vt")
			this->texCoords.push_back(
				{std::stof(input[1]), std::stof(input[2])});
		else if (input[0] == "f")
			this->parseFace(input);
		input.clear();
	}
}

Face Model::makeFace(std::vector<std::string> const &input, std::size_t v1,
					 std::size_t v2, std::size_t v3) {
	static std::vector<std::string> tmp;
	Face face;
	if (input[1].find("//") != std::string::npos) {
		face = vertexNormal(input, v1, v2, v3);
		return (face);
	}
	split(input[1], "/", tmp);
	switch (tmp.size()) {
		case 1:
			face = vertexOnly(input, v1, v2, v3);
			break;
		case 2:
			face = vertexTexture(input, v1, v2, v3);
			break;
		case 3:
			face = vertexTextureNormal(input, v1, v2, v3);
			break;
		default:
			throw std::runtime_error("Bad file format !");
	}
	tmp.clear();
	return (face);
}

void Model::parseFace(std::vector<std::string> const &input) {
	std::size_t len = input.size();
	if (len != 4 && len != 5)
		throw std::runtime_error("Face format not handled !");

	this->faces.push_back(makeFace(input, 3, 2, 1));
	if (len == 5)
		this->faces.push_back(makeFace(input, 3, 4, 1));
}

Face Model::vertexOnly(std::vector<std::string> const &input, std::size_t v1,
					   std::size_t v2, std::size_t v3) {
	Face res;

	res.vertices[0] = getVertice(std::stoi(input[v1]));
	res.vertices[1] = getVertice(std::stoi(input[v2]));
	res.vertices[2] = getVertice(std::stoi(input[v3]));

	res.texCoords[0] = {res.vertices[0].x + 1.0f, res.vertices[0].y + 1.0f};
	res.texCoords[1] = {res.vertices[1].x + 1.0f, res.vertices[1].y + 1.0f};
	res.texCoords[2] = {res.vertices[2].x + 1.0f, res.vertices[2].y + 1.0f};

	vec3f dir = (res.vertices[2] - res.vertices[0])
					.cross(res.vertices[1] - res.vertices[0]);

	res.normal = dir / dir.len();
	res.color = this->colorsPalette[this->faces.size() % 4];

	return (res);
}

Face Model::vertexTexture(std::vector<std::string> const &input, std::size_t v1,
						  std::size_t v2, std::size_t v3) {
	std::cout << "VERTEX TEXTURE\n";
	Face res;
	return (res);
}

Face Model::vertexTextureNormal(std::vector<std::string> const &input,
								std::size_t v1, std::size_t v2,
								std::size_t v3) {
	Face res;

	std::size_t indices[] = {v1, v2, v3};
	std::vector<std::string> tmp;
	for (std::size_t i = 0; i < 3; ++i) {
		split(input[indices[i]], "/", tmp);
		res.vertices[i] = getVertice(std::stoi(tmp[0]));
		res.texCoords[i] = getTexCoord(std::stoi(tmp[1]));
		res.normal = getNormal(std::stoi(tmp[2]));
		tmp.clear();
	}
	res.color = this->colorsPalette[this->faces.size() % 4];
	return (res);
}

Face Model::vertexNormal(std::vector<std::string> const &input, std::size_t v1,
						 std::size_t v2, std::size_t v3) {
	std::cout << "VERTEX NORMAL\n";
	Face res;
	return (res);
}

std::vector<Face> const &Model::getFaces() const {
	return (this->faces);
}

vec3f const &Model::getVertice(std::size_t i) const {
	return (this->vertices[i - 1]);
}

vec3f const &Model::getNormal(std::size_t i) const {
	return (this->normals[i - 1]);
}

vec2f const &Model::getTexCoord(std::size_t i) const {
	return (this->texCoords[i - 1]);
}
