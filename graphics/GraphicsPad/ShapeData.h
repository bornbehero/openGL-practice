#pragma once
#include <GL\glew.h>
#include <Vertex.h>

class Shapedata
{
public:
	Shapedata() :
		vertices(0), numVertices(0),
		Indices(0), numIndices(0)
	{}

	GLsizeiptr VertexBufferSize() const
	{
		return (numVertices * sizeof(Vertex));
	}

	GLsizeiptr IndicesBufferSize() const
	{
		return (numIndices * sizeof(GLuint));
	}

	void cleanup()
	{
		delete[] vertices;
		delete[] Indices;
		numVertices = numIndices = 0;

	}

	Vertex* vertices;
	GLuint  numVertices;
	GLushort*  Indices;
	GLuint numIndices;


};