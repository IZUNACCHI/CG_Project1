#include "loader_wavefront.hpp"
#include "vboindexer.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <vector>

#include <iostream>

class Object
{
public:
	Object(const char* filename, std::vector<Object>& objects, unsigned int& objectCount, const char* texfile);

	void translate(float pos_x, float pos_y, float pos_z);
	void rotate_y(float rad);
	void rotate_x(float rad);
	void rotate_z(float rad);
	void scale(float scale_x, float scale_y, float scale_z);

	glm::mat4 model();
	size_t vertexBufferSize();
	float* vertices();
	size_t indexBufferSize();
	unsigned int* indices();

	const char* texture();
	
	~Object() {};

private:
	glm::mat4 m_model;
	std::vector<float> in_vertices;
	std::vector<float> out_vertices;
	std::vector<unsigned int> out_indices;
	const char* m_texture;
};