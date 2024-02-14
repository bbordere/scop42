#include "File3D.hpp"

void File3D::load(std::string const &path) {
	this->vertices.clear();
	this->normals.clear();
	this->texCoords.clear();
	this->faces.clear();
	this->isFileTextured = false;
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
		split(line, ' ', input);
		if (input.empty())
			continue;

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

void File3D::computeNormals(Face &face) {
	vec3f dir = (face.vertices[2] - face.vertices[0])
					.cross(face.vertices[1] - face.vertices[0]);

	face.normals[0] = (dir / dir.len()) * -1;
	face.normals[1] = (dir / dir.len()) * -1;
	face.normals[2] = (dir / dir.len()) * -1;
}

void File3D::computeUV(Face &face) {
	vec3f s1 = face.vertices[1] - face.vertices[0];
	vec3f s2 = face.vertices[2] - face.vertices[1];
	vec3f norm = vec3f::cross(s1, s2).normalize();
	norm.x = std::abs(norm.x);
	norm.y = std::abs(norm.y);
	norm.z = std::abs(norm.z);

	if (norm.x >= norm.z && norm.x >= norm.y) // x plane
	{
		face.texCoords[0] = vec2f(face.vertices[0].z, face.vertices[0].y);
		face.texCoords[1] = vec2f(face.vertices[1].z, face.vertices[1].y);
		face.texCoords[2] = vec2f(face.vertices[2].z, face.vertices[2].y);
	}
	else if (norm.z >= norm.x && norm.z >= norm.y) // z plane
	{
		face.texCoords[0] = vec2f(face.vertices[0].x, face.vertices[0].y);
		face.texCoords[1] = vec2f(face.vertices[1].x, face.vertices[1].y);
		face.texCoords[2] = vec2f(face.vertices[2].x, face.vertices[2].y);
	}
	else if (norm.y >= norm.x && norm.y >= norm.z) // y plane
	{
		face.texCoords[0] = vec2f(face.vertices[0].x, face.vertices[0].z);
		face.texCoords[1] = vec2f(face.vertices[1].x, face.vertices[1].z);
		face.texCoords[2] = vec2f(face.vertices[2].x, face.vertices[2].z);
	}
}

Face File3D::makeFace(std::vector<std::string> const &input, std::size_t v1,
					  std::size_t v2, std::size_t v3) {
	Face face;
	if (input[1].find("//") != std::string::npos) {
		face = vertexNormal(input, v1, v2, v3);
		return (face);
	}
	static std::vector<std::string> tmp;
	split(input[1], '/', tmp);
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

	for (std::size_t i = 1; i < len - 2; ++i) {
		this->faces.emplace_back(makeFace(input, 1, i + 1, i + 2));
	}
}

Face File3D::vertexOnly(std::vector<std::string> const &input, std::size_t v1,
						std::size_t v2, std::size_t v3) {
	Face res;

	res.vertices[0] = getVertice(std::stoi(input[v1]));
	res.vertices[1] = getVertice(std::stoi(input[v2]));
	res.vertices[2] = getVertice(std::stoi(input[v3]));

	computeNormals(res);
	computeUV(res);

	res.color = this->colorsPalette[this->faces.size() % 4];

	return (res);
}

Face File3D::vertexTexture(std::vector<std::string> const &input,
						   std::size_t v1, std::size_t v2, std::size_t v3) {
	Face res;

	std::size_t indices[] = {v1, v2, v3};
	std::vector<std::string> tmp;
	for (std::size_t i = 0; i < 3; ++i) {
		split(input[indices[i]], '/', tmp);
		res.vertices[i] = getVertice(std::stoi(tmp[0]));
		res.texCoords[i] = getTexCoord(std::stoi(tmp[1]));
		tmp.clear();
	}
	res.color = this->colorsPalette[this->faces.size() % 4];
	this->isFileTextured = true;
	return (res);
}

Face File3D::vertexTextureNormal(std::vector<std::string> const &input,
								 std::size_t v1, std::size_t v2,
								 std::size_t v3) {
	Face res;

	std::size_t indices[] = {v1, v2, v3};
	static std::vector<std::string> tmp;
	for (std::size_t i = 0; i < 3; ++i) {
		split(input[indices[i]], '/', tmp);
		res.vertices[i] = getVertice(std::stoi(tmp[0]));
		res.texCoords[i] = getTexCoord(std::stoi(tmp[1]));
		res.normals[i] = getNormal(std::stoi(tmp[2]));
		tmp.clear();
	}
	res.color = this->colorsPalette[this->faces.size() % 4];
	this->isFileTextured = true;
	return (res);
}

Face File3D::vertexNormal(std::vector<std::string> const &input, std::size_t v1,
						  std::size_t v2, std::size_t v3) {
	Face res;

	std::size_t indices[] = {v1, v2, v3};
	std::vector<std::string> tmp;
	for (std::size_t i = 0; i < 3; ++i) {
		split(input[indices[i]], '/', tmp);
		res.vertices[i] = getVertice(std::stoi(tmp[0]));
		res.normals[i] = getNormal(std::stoi(tmp[1]));
		computeUV(res);
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

bool File3D::getTexturedStatus() const {
	return (this->isFileTextured);
}
