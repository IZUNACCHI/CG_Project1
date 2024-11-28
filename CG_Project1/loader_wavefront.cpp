#include "loader_wavefront.hpp"

std::vector<std::string> Split(std::string line, std::string delimiter) {

	std::vector<std::string> splitLine;
	size_t pos = 0;
	std::string token;

	while ((pos = line.find(delimiter)) != std::string::npos)
	{
		token = line.substr(0, pos);
		splitLine.push_back(token);
		line.erase(0, pos + delimiter.size());
	}
	splitLine.push_back(line);

	return splitLine;
}

glm::vec2 ReadVec2(std::vector<std::string> words) {
	return glm::vec2(std::stof(words[1]), std::stof(words[2]));
}

glm::vec3 ReadVec3(std::vector<std::string> words) {
	return glm::vec3(std::stof(words[1]), std::stof(words[2]), std::stof(words[3]));
}

void ReadCorner(std::string description, std::vector<glm::vec3>& v,
	std::vector<glm::vec2>& vt, std::vector<glm::vec3>& vn, std::vector<float>& vertices) {

	std::vector<std::string> v_vt_vn = Split(description, "/");

	glm::vec3 pos = v[std::stol(v_vt_vn[0]) - 1];
	vertices.push_back(pos[0]);
	vertices.push_back(pos[1]);
	vertices.push_back(pos[2]);

	glm::vec2 tex = vt[std::stol(v_vt_vn[1]) - 1];
	vertices.push_back(tex[0]);
	vertices.push_back(tex[1]);

	glm::vec3 normal = vn[std::stol(v_vt_vn[2]) - 1];
	vertices.push_back(normal[0]);
	vertices.push_back(normal[1]);
	vertices.push_back(normal[2]);
}

void ReadFace(std::vector<std::string> words, std::vector<glm::vec3>& v,
	std::vector<glm::vec2>& vt, std::vector<glm::vec3>& vn, std::vector<float>& vertices) {

	size_t triangleCount = words.size() - 3;

	for (size_t i = 0; i < triangleCount; i++)
	{
		ReadCorner(words[1], v, vt, vn, vertices);
		ReadCorner(words[2 + i], v, vt, vn, vertices);
		ReadCorner(words[3 + i], v, vt, vn, vertices);
	}
}

void LoadObj(const char* filename, std::vector<float>& vertices) {

	std::vector<glm::vec3> v;
	std::vector<glm::vec2> vt;
	std::vector<glm::vec3> vn;

	size_t vertexCount = 0;
	size_t texcoordCount = 0;
	size_t normalCount = 0;
	size_t triangleCount = 0;

	std::string line;
	std::vector<std::string> words;

	std::ifstream file;
	file.open(filename);
	while (std::getline(file, line)) {
		words = Split(line, " ");

		if (!words[0].compare("v"))
		{
			vertexCount++;
		}
		else if (!words[0].compare("vt"))
		{
			texcoordCount++;
		}
		else if (!words[0].compare("vn"))
		{
			normalCount++;
		}
		else if (!words[0].compare("f"))
		{
			triangleCount += words.size() - 3;
		}
	}
	file.close();

	v.reserve(vertexCount);
	vt.reserve(texcoordCount);
	vn.reserve(normalCount);
	vertices.reserve(triangleCount * 3 * 8); // how many triangles * number of corners in a triangle * number of floats in a corner

	file.open(filename);
	while (std::getline(file, line)) {
		words = Split(line, " ");

		if (!words[0].compare("v"))
		{
			v.push_back(ReadVec3(words));
		}
		else if (!words[0].compare("vt"))
		{
			vt.push_back(ReadVec2(words));
		}
		else if (!words[0].compare("vn"))
		{
			vn.push_back(ReadVec3(words));
		}
		else if (!words[0].compare("f"))
		{
			ReadFace(words, v, vt, vn, vertices);
		}
	}
	file.close();
}