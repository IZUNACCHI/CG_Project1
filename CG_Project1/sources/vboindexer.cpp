#include "vboindexer.hpp"
#include <iostream>

// Returns true if v1 can be considered equal to v2
bool is_near(float v1, float v2) {
	return fabs(v1 - v2) < 0.01f;
}

// Searches through all already-exported vertices
// for a similar one.
// Similar = same position + same texcoords + same normal
// Returns true is a similar vertex was found, false if not.
// If true, passes the index (result) of the similar vertex.
bool GetSimilarVertexIndex(
	std::vector<float>& pckd_vertex,
	std::vector<float>& out_vbo,
	unsigned int& result
) {
	unsigned int index = 0;
	for (unsigned int i = 0; i < out_vbo.size(); i+=8) {
		if (
			is_near(pckd_vertex[0], out_vbo[i]) &&
			is_near(pckd_vertex[1], out_vbo[i+1])&&
			is_near(pckd_vertex[2], out_vbo[i+2])&&
			is_near(pckd_vertex[3], out_vbo[i+3])&&
			is_near(pckd_vertex[4], out_vbo[i+4])&&
			is_near(pckd_vertex[5], out_vbo[i+5])&&
			is_near(pckd_vertex[6], out_vbo[i+6])&&
			is_near(pckd_vertex[7], out_vbo[i+7])
			) {
			result = index;
			return true;
		}
		index++;
	}
	return false;
}

void PackVertices(std::vector<float>& in_vertices, std::vector<std::vector<float>>& pckd_vertices)
{
	pckd_vertices.reserve(in_vertices.size() / 8);
	std::vector<float> temp_pckd_v(8);
	for (unsigned int i = 0; i < in_vertices.size(); i += 8)
	{
		//position
		temp_pckd_v[0] = in_vertices[i];
		temp_pckd_v[1] = in_vertices[i+1];
		temp_pckd_v[2] = in_vertices[i+2];
		//texcoords
		temp_pckd_v[3] = in_vertices[i+3];
		temp_pckd_v[4] = in_vertices[i+4];
		//normals
		temp_pckd_v[5] = in_vertices[i+5];
		temp_pckd_v[6] = in_vertices[i+6];
		temp_pckd_v[7] = in_vertices[i+7];

		pckd_vertices.push_back(temp_pckd_v);
	}
}

void IndexVBO(
	std::vector<float>& in_vertices,
	std::vector<float>& out_vbo,
	std::vector<unsigned int>& out_indices
) {
	std::vector<std::vector<float>> pckd_vertices;
	std::vector<float> pckd_vertex;
	
	// Pack all each input vertices' attributes into single vertices.
	PackVertices(in_vertices, pckd_vertices);
	out_indices.reserve(pckd_vertices.size());

	unsigned int newCap_base = 8;
	for (unsigned int i = 0; i < pckd_vertices.size(); i++)
	{
		pckd_vertex = pckd_vertices[i];
		unsigned int index;
		bool found = GetSimilarVertexIndex(pckd_vertex, out_vbo, index);

		if (found) { // If a similar vertex is already in the VBO, use it instead.
			out_indices.push_back(index);
		}
		else { // If not, it needs to be added in the output data.
			out_vbo.reserve(i > 0 ? newCap_base * i : newCap_base);
			for (unsigned int j = 0; j < pckd_vertex.size(); j++)
			{
				out_vbo.push_back(pckd_vertex[j]);
			}
			out_indices.push_back((unsigned int)out_vbo.size() / 8 - 1);
		}
	}
}
