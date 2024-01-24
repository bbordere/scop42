#include "File3D.hpp"

void File3D::load(std::string const &path) {
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
	this->computeCenter();
	float min[] = {std::numeric_limits<float>::max(),
				   std::numeric_limits<float>::max()};
	float max[] = {std::numeric_limits<float>::min(),
				   std::numeric_limits<float>::min()};
	for (std::size_t i = 0; i < this->vertices.size(); ++i) {
		min[0] = std::min(this->vertices[i].x, min[0]);
		min[1] = std::min(this->vertices[i].y, min[1]);
		max[0] = std::max(this->vertices[i].x, max[0]);
		max[1] = std::max(this->vertices[i].y, max[1]);
	}

	float dX = 1 / (max[0] - min[0]);
	float dY = 1 / (max[1] - min[1]);
	for (auto f : this->faces) {
		for (int i = 0; i < 3; ++i) {
			// f.texCoords[i].x = (f.vertices[i].x - min[0]) * dX;
			// f.texCoords[i].y = (f.vertices[i].y - min[1]) * dY;
			std::cout << f.texCoords[i] << '\n';
			std::cout << f.vertices[i] << '\n';
		}
		std::cout << '\n';
	}
}

void File3D::computeCenter() {
	float min[] = {std::numeric_limits<float>::max(),
				   std::numeric_limits<float>::max(),
				   std::numeric_limits<float>::max()};
	float max[] = {std::numeric_limits<float>::min(),
				   std::numeric_limits<float>::min(),
				   std::numeric_limits<float>::min()};
	for (std::size_t i = 0; i < this->vertices.size(); ++i) {
		min[0] = std::min(this->vertices[i].x, min[0]);
		min[1] = std::min(this->vertices[i].y, min[1]);
		min[2] = std::min(this->vertices[i].z, min[2]);
		max[0] = std::max(this->vertices[i].x, max[0]);
		max[1] = std::max(this->vertices[i].y, max[1]);
		max[2] = std::max(this->vertices[i].z, max[2]);
	}
	this->boundVector = {
		max[0] - min[0],
		max[1] - min[1],
		max[2] - min[2],
	};
	this->center = vec3f((min[0] + max[0]) / 2, (min[1] + max[1]) / 2,
						 (min[2] + max[2]) / 2);
}

vec3f const &File3D::getBoundVec() const {
	return (this->boundVector);
}

Face File3D::makeFace(std::vector<std::string> const &input, std::size_t v1,
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

void File3D::parseFace(std::vector<std::string> const &input) {
	std::size_t len = input.size();
	if (len != 4 && len != 5)
		throw std::runtime_error("Face format not handled !");

	this->faces.push_back(makeFace(input, 3, 2, 1));
	if (len == 5)
		this->faces.push_back(makeFace(input, 1, 4, 3));
}

Face File3D::vertexOnly(std::vector<std::string> const &input, std::size_t v1,
						std::size_t v2, std::size_t v3) {
	Face res;

	res.vertices[0] = getVertice(std::stoi(input[v1]));
	res.vertices[1] = getVertice(std::stoi(input[v2]));
	res.vertices[2] = getVertice(std::stoi(input[v3]));

	// res.texCoords[0] = vec2f(res.vertices[0].x, res.vertices[0].y);
	// res.texCoords[1] = vec2f(res.vertices[1].x, res.vertices[1].y);
	// res.texCoords[2] = vec2f(res.vertices[2].x, res.vertices[2].y);

	res.texCoords[0] = vec2f(0, 1);
	res.texCoords[1] = vec2f(1, 1);
	res.texCoords[2] = vec2f(1, 0);

	float min[] = {std::numeric_limits<float>::max(),
				   std::numeric_limits<float>::max(),
				   std::numeric_limits<float>::max()};
	float max[] = {std::numeric_limits<float>::min(),
				   std::numeric_limits<float>::min(),
				   std::numeric_limits<float>::min()};
	for (std::size_t i = 0; i < this->vertices.size(); ++i) {
		min[0] = std::min(this->vertices[i].x, min[0]);
		min[1] = std::min(this->vertices[i].y, min[1]);
		min[2] = std::min(this->vertices[i].z, min[2]);
		max[0] = std::max(this->vertices[i].x, max[0]);
		max[1] = std::max(this->vertices[i].y, max[1]);
		max[2] = std::max(this->vertices[i].z, max[2]);
	}

	float dX = 1 / (max[0] - min[0]);
	float dY = 1 / (max[1] - min[1]);
	res.texCoords[0] = vec2f((res.vertices[0].x - min[0]) * dX,
							 (res.vertices[0].y - min[1]) * dY);

	res.texCoords[1] = vec2f((res.vertices[1].x - min[0]) * dX,
							 (res.vertices[1].y - min[1]) * dY);

	res.texCoords[2] = vec2f((res.vertices[2].x - min[0]) * dX,
							 (res.vertices[2].y - min[1]) * dY);

	// res.texCoords[1].x = (res.vertices[1].x - min[0]) * dX;
	// res.texCoords[1].y = (res.vertices[1].y - min[1]) * dY;

	// res.texCoords[2].x = (res.vertices[2].x - min[0]) * dX;
	// res.texCoords[2].y = (res.vertices[2].y - min[1]) * dY;

	std::cout << res.texCoords[0] << '\n';
	std::cout << res.texCoords[1] << '\n';
	std::cout << res.texCoords[2] << '\n';

	vec3f dir = (res.vertices[2] - res.vertices[0])
					.cross(res.vertices[1] - res.vertices[0]);

	res.normals[0] = dir / dir.len();
	res.normals[1] = dir / dir.len();
	res.normals[2] = dir / dir.len();
	res.color = this->colorsPalette[this->faces.size() % 4];

	return (res);
}

Face File3D::vertexTexture(std::vector<std::string> const &input,
						   std::size_t v1, std::size_t v2, std::size_t v3) {
	std::cout << "VERTEX TEXTURE\n";
	Face res;
	return (res);
}

Face File3D::vertexTextureNormal(std::vector<std::string> const &input,
								 std::size_t v1, std::size_t v2,
								 std::size_t v3) {
	Face res;

	std::size_t indices[] = {v1, v2, v3};
	std::vector<std::string> tmp;
	for (std::size_t i = 0; i < 3; ++i) {
		split(input[indices[i]], "/", tmp);
		res.vertices[i] = getVertice(std::stoi(tmp[0]));
		res.texCoords[i] = getTexCoord(std::stoi(tmp[1]));
		res.normals[i] = getNormal(std::stoi(tmp[2]));
		tmp.clear();
	}
	res.color = this->colorsPalette[this->faces.size() % 4];
	return (res);
}

Face File3D::vertexNormal(std::vector<std::string> const &input, std::size_t v1,
						  std::size_t v2, std::size_t v3) {
	Face res;

	std::size_t indices[] = {v1, v2, v3};
	std::vector<std::string> tmp;
	for (std::size_t i = 0; i < 3; ++i) {
		split(input[indices[i]], "/", tmp);
		res.vertices[i] = getVertice(std::stoi(tmp[0]));
		res.normals[i] = getNormal(std::stoi(tmp[1]));
		res.texCoords[i] = {0, 0};
		tmp.clear();
	}
	res.color = this->colorsPalette[this->faces.size() % 4];
	return (res);
}

std::vector<Face> const &File3D::getFaces() const {
	return (this->faces);
}

vec3f const &File3D::getVertice(std::size_t i) const {
	return (this->vertices[i - 1]);
}

vec3f const &File3D::getNormal(std::size_t i) const {
	return (this->normals[i - 1]);
}

vec2f const &File3D::getTexCoord(std::size_t i) const {
	return (this->texCoords[i - 1]);
}

vec3f const &File3D::getCenter() const {
	return (this->center);
}
