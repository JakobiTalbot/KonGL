#include "Mesh.h"
#include <gl_core_4_4.h>
Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

void Mesh::Initialise(unsigned int nVertexCount, const Vertex* pVertices, unsigned int nIndexCount, unsigned int* pIndices)
{
	assert(vao == 0);

	// generate buffers
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	// bind stuff
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// fill vertex buffer
	glBufferData(GL_ARRAY_BUFFER, nVertexCount * sizeof(Vertex), pVertices, GL_STATIC_DRAW);

	// enable first element as position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// bind any existing indices
	if (nIndexCount != 0)
	{
		glGenBuffers(1, &ibo);

		// bind vertex buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		// fill vertex buffer
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, nIndexCount * sizeof(unsigned int), pIndices, GL_STATIC_DRAW);
	}
	triCount = nIndexCount / 3;

	// enable third element as texture
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)32);

	//unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::InitQuad()
{
	assert(vao == 0);

	// generate buffers
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	//bind stuff
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// define verticles for 2 triangles
	Vertex vertices[6];
	vertices[0].v4Position = { -0.5f, 0, 0.5f, 1 };
	vertices[1].v4Position = { 0.5f, 0, 0.5f, 1 };
	vertices[2].v4Position = { -0.5f, 0, -0.5f, 1 };

	vertices[3].v4Position = { -0.5f, 0, -0.5f, 1 };
	vertices[4].v4Position = { 0.5f, 0, 0.5f, 1 };
	vertices[5].v4Position = { 0.5f, 0, -0.5f, 1 };


	// define texture coordinates
	vertices[0].v2TexCoord = { 0, 1 };
	vertices[1].v2TexCoord = { 1, 1 };
	vertices[2].v2TexCoord = { 0, 0 };

	vertices[3].v2TexCoord = { 0, 0 };
	vertices[4].v2TexCoord = { 1, 1 };
	vertices[5].v2TexCoord = { 1, 0 };

	// fill vertex buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 6, vertices, GL_STATIC_DRAW);

	// enable first element as position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// enable third element as texture
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)32);

	// unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// quad has 2 triangles
	triCount = 2;
}

void Mesh::Draw()
{
	glBindVertexArray(vao);

	// using indices or vertices?
	if (ibo != 0)
		glDrawElements(GL_TRIANGLES, 3 * triCount, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, 3 * triCount);
}