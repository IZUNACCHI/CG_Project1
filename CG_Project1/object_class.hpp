#include "loader_wavefront.hpp"
#include "vboindexer.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <vector>

class Object
{
public:
	Object(const char* filename, std::vector<Object>& objects, unsigned int& objectCount, const char* texfile);
	Object(const char* filename, std::vector<Object>& objects, unsigned int& objectCount, const char* texfile, glm::vec3& pos);

	void rotate_x(float& rad);
	void rotate_y(float& rad);
	void rotate_z(float& rad);
	void scale(glm::vec3& scale);

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