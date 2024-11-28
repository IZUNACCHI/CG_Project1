#include "object_class.hpp"

Object::Object(const char* filename, std::vector<Object>& objects, unsigned int& objectCount) 
{
	LoadObj(filename, in_vertices);
	IndexVBO(in_vertices, out_vertices, out_indices);
	glm::mat4 model(1.0f);
	m_model = model;
	m_model = glm::rotate(m_model, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
	objectCount++;
	objects.reserve(objectCount);
}

Object::Object(const char* filename, std::vector<Object>& objects, unsigned int& objectCount, glm::vec3& pos)
{
	LoadObj(filename, in_vertices);
	IndexVBO(in_vertices, out_vertices, out_indices);
	glm::mat4 model(1.0f);
	m_model = model;
	m_model = glm::translate(m_model, pos);
	m_model = glm::rotate(m_model, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
	objectCount++;
	objects.reserve(objectCount);
}

inline void Object::rotate_x(float& rad)
{
	m_model = glm::rotate(m_model, glm::radians(rad), glm::vec3(1.0, 0.0, 0.0));
}

inline void Object::rotate_y(float& rad)
{
	m_model = glm::rotate(m_model, glm::radians(rad), glm::vec3(0.0, 1.0, 0.0));
}

inline void Object::rotate_z(float& rad)
{
	m_model = glm::rotate(m_model, glm::radians(rad), glm::vec3(0.0, 0.0, 1.0));
}

inline void Object::scale(glm::vec3& scale)
{
	m_model = glm::scale(m_model, scale);
}

inline glm::mat4 Object::model()
{
	return m_model;
}

inline size_t Object::vertexBufferSize()
{
	return out_vertices.size();
}

inline float* Object::vertices()
{
	return out_vertices.data();
}

inline size_t Object::indexBufferSize()
{
	return out_indices.size();
}

inline unsigned int* Object::indices()
{
	return out_indices.data();
}
