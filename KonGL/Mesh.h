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

	/*
		@brief Initialises a mesh with specified variables
		@param Number of vertices for the mesh to have
		@param Vertices to fill the vertex buffer with
		@param Number of indices for the mesh to have
		@param Indices to fill the vertex buffer with
	*/
	void Initialise(unsigned int nVertexCount, const Vertex* pVertices, unsigned int nIndexCount = 0, unsigned int* pIndices = nullptr);

	/*
		@brief Creates the mesh as a fullscreen quad
	*/
	void InitialiseFullscreenQuad();

	/*
		@brief Initialises a quad
	*/
	void InitQuad();
	
	/*
		@brief Draws the mesh
	*/
	virtual void Draw();

protected:
	unsigned int triCount;
	unsigned int vao, vbo, ibo;
};