#include "loader_wavefront.hpp"
#include "vboindexer.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Object
{
public:
	Object(const char* filename, std::vector<Object>& objects, unsigned int& objectCount);
	Object(const char* filename, std::vector<Object>& objects, unsigned int& objectCount, glm::vec3& pos);

	inline void rotate_x(float& rad);
	inline void rotate_y(float& rad);
	inline void rotate_z(float& rad);
	inline void scale(glm::vec3& scale);

	inline glm::mat4 model();
	inline size_t vertexBufferSize();
	inline float* vertices();
	inline size_t indexBufferSize();
	inline unsigned int* indices();
	
	~Object() {};

private:
	glm::mat4 m_model;
	std::vector<float> in_vertices;
	std::vector<float> out_vertices;
	std::vector<unsigned int> out_indices;
};