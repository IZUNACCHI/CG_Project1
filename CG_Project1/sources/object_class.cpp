#include "object_class.hpp"

Object::Object(const char* filename, std::vector<Object>& objects, unsigned int& objectCount, const char* texfile)
	: m_model{ glm::mat4(1.0f) }, m_texture{ texfile }
{
	LoadObj(filename, in_vertices);
	IndexVBO(in_vertices, out_vertices, out_indices);
	objectCount++;
	objects.reserve(objectCount);
}
void Object::translate(float pos_x, float pos_y, float pos_z)
{
	m_model = glm::translate(m_model, glm::vec3(pos_x, pos_y, pos_z));
}

void Object::rotate_x(float rad)
{
	m_model = glm::rotate(m_model, glm::radians(rad), glm::vec3(1.0, 0.0, 0.0));
}

void Object::rotate_y(float rad)
{
	m_model = glm::rotate(m_model, glm::radians(rad), glm::vec3(0.0, 1.0, 0.0));
}

void Object::rotate_z(float rad)
{
	m_model = glm::rotate(m_model, glm::radians(rad), glm::vec3(0.0, 0.0, 1.0));
}

void Object::scale(float scale_x, float scale_y, float scale_z)
{
	m_model = glm::scale(m_model, glm::vec3(scale_x, scale_y, scale_z));
}

glm::mat4 Object::model()
{
	return m_model;
}

size_t Object::vertexBufferSize()
{
	return out_vertices.size();
}

float* Object::vertices()
{
	return out_vertices.data();
}

size_t Object::indexBufferSize()
{
	return out_indices.size();
}

unsigned int* Object::indices()
{
	return out_indices.data();
}

const char* Object::texture()
{
	return m_texture;
}

