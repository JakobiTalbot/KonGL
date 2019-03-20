#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>
class Mesh
{
public:
	Mesh() : triCount(0), vao(0), vbo(0), ibo(0) {}
	virtual ~Mesh();

	struct Vertex
	{
		glm::vec4 v4Position, v4Normal;
		glm::vec2 v2TexCoord;
	};

	void Initialise(unsigned int nVertexCount, const Vertex* pVertices, unsigned int nIndexCount = 0, unsigned int* pIndices = nullptr);

	void InitQuad();
	
	virtual void Draw();

protected:
	unsigned int triCount;
	unsigned int vao, vbo, ibo;
};