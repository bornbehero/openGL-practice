#include <ShapeFactory.h>
#include <glm\glm.hpp>
#include <Vertex.h>

#define NUM_ELEMENT(x) sizeof(x)/sizeof(*x)

using glm::vec3;
using glm::vec2;
using glm::vec4;

glm::vec3 randomColor()
{
	glm::vec3 ret;
	ret.x = rand() / (float)RAND_MAX;
	ret.y = rand() / (float)RAND_MAX;
	ret.z = rand() / (float)RAND_MAX;
	return ret;
}

Shapedata ShapeFactory::MakeTriangle()
{
	Shapedata data;

	Vertex	Triangle[] =
	{
		glm::vec3(+0.0f,+1.0f,+0.0f),
		glm::vec3(+1.0f,+0.0f,+0.0f),
		glm::vec3(+0.0f,+0.0f,+1.0f),
		glm::vec2(+0.0f,+0.0f),
		vec4(-1.0f,0.0f,0.0f,1.0f),//tengent

		glm::vec3(-1.0f,-1.0f,+0.0f),
		glm::vec3(+0.0f,+1.0f,+0.0f),
		glm::vec3(+0.0f,+0.0f,+1.0f),
		glm::vec2(+0.5f,+1.0f),
		vec4(-1.0f,0.0f,0.0f,1.0f),//tengent

		glm::vec3(+1.0f,-1.0f,+0.0f),
		glm::vec3(+0.0f,+0.0f,+1.0f),
		glm::vec3(+0.0f,+0.0f,+1.0f),
		glm::vec2(+1.0f,+0.0f),
		vec4(-1.0f,0.0f,0.0f,1.0f),//tengent
	};

	data.numVertices = NUM_ELEMENT(Triangle);
	data.vertices = new Vertex[data.numVertices];
	memcpy(data.vertices, Triangle, sizeof(Triangle));

	GLushort Indices[] =
	{
		0,1,2
	};

	data.numIndices = NUM_ELEMENT(Indices);
	data.Indices = new GLushort[data.numIndices];
	memcpy(data.Indices, Indices, sizeof(Indices));

	return data;

}

Shapedata ShapeFactory::MakeCube() {
	Shapedata data;
	Vertex CubeVerts[] = {
		vec3(-1.0f, +1.0f, +1.0f), // 0
		vec3(+1.0f, +0.0f, +0.0f), // Color
		vec3(+0.0f, +1.0f, +0.0f), // normal
		vec2(+0.0f, +0.0f),		   //uv
		vec4(+1.0f,0.0f,0.0f,1.0f),//tengent
		vec3(+1.0f, +1.0f, +1.0f), // 1
		vec3(+0.0f, +1.0f, +0.0f), // Color
		vec3(+0.0f, +1.0f, +0.0f), // normal
		vec2(+1.0f, +0.0f),		   //uv
		vec4(+1.0f,0.0f,0.0f,1.0f),//tengent
		vec3(+1.0f, +1.0f, -1.0f), // 2
		vec3(+0.0f, +0.0f, +1.0f), // Color
		vec3(+0.0f, +1.0f, +0.0f), // normal
		vec2(+1.0f, +1.0f),		   //uv
		vec4(+1.0f,0.0f,0.0f,1.0f),//tengent
		vec3(-1.0f, +1.0f, -1.0f), // 3
		vec3(+1.0f, +1.0f, +1.0f), // Color
		vec3(+0.0f, +1.0f, +0.0f), // normal
		vec2(+0.0f, +1.0f),		   //uv
		vec4(+1.0f,0.0f,0.0f,1.0f),//tengent

		vec3(-1.0f, +1.0f, -1.0f), // 4
		vec3(+1.0f, +0.0f, +1.0f), // Color
		vec3(+0.0f, +0.0f, -1.0f), // normal
		vec2(+1.0f, +1.0f),		   //uv
		vec4(-1.0f,0.0f,0.0f,1.0f),//tengent
		vec3(+1.0f, +1.0f, -1.0f), // 5
		vec3(+0.0f, +0.5f, +0.2f), // Color
		vec3(+0.0f, +0.0f, -1.0f), // normal
		vec2(+0.0f, +1.0f),		   //uv
		vec4(-1.0f,0.0f,0.0f,1.0f),//tengent
		vec3(+1.0f, -1.0f, -1.0f), // 6
		vec3(+0.8f, +0.6f, +0.4f), // Color
		vec3(+0.0f, +0.0f, -1.0f), // normal
		vec2(+0.0f, +0.0f),		   //uv
		vec4(-1.0f,0.0f,0.0f,1.0f),//tengent
		vec3(-1.0f, -1.0f, -1.0f), // 7
		vec3(+0.3f, +1.0f, +0.5f), // Color
		vec3(+0.0f, +0.0f, -1.0f), // normal
		vec2(+1.0f, +0.0f),		   //uv
		vec4(-1.0f,0.0f,0.0f,1.0f),//tengent

		vec3(+1.0f, +1.0f, -1.0f), // 8
		vec3(+0.2f, +0.5f, +0.2f), // Color
		vec3(+1.0f, +0.0f, -0.0f), // normal
		vec2(+1.0f, +1.0f),		   //uv
		vec4(-0.0f,+0.0f,-1.0f,1.0f),//tengent
		vec3(+1.0f, +1.0f, +1.0f), // 9
		vec3(+0.9f, +0.3f, +0.7f), // Color
		vec3(+1.0f, +0.0f, -0.0f), // normal
		vec2(+0.0f, +1.0f),		   //uv
		vec4(-0.0f,+0.0f,-1.0f,1.0f),//tengent
		vec3(+1.0f, -1.0f, +1.0f), // 10
		vec3(+0.3f, +0.7f, +0.5f), // Color
		vec3(+1.0f, +0.0f, -0.0f), // normal
		vec2(+0.0f, +0.0f),		   //uv
		vec4(-0.0f,+0.0f,-1.0f,1.0f),//tengent
		vec3(+1.0f, -1.0f, -1.0f), // 11
		vec3(+0.5f, +0.7f, +0.5f), // Color
		vec3(+1.0f, +0.0f, -0.0f), // normal
		vec2(+1.0f, +0.0f),		   //uv
		vec4(-0.0f,+0.0f,-1.0f,1.0f),//tengent

		vec3(-1.0f, +1.0f, +1.0f), // 12
		vec3(+0.7f, +0.8f, +0.2f), // Color
		vec3(-1.0f, +0.0f, -0.0f), // normal
		vec2(+1.0f, +1.0f),		   //uv
		vec4(-0.0f,+0.0f,1.0f,1.0f),//tengent
		vec3(-1.0f, +1.0f, -1.0f), // 13
		vec3(+0.5f, +0.7f, +0.3f), // Color
		vec3(-1.0f, +0.0f, -0.0f), // normal
		vec2(+0.0f, +1.0f),		   //uv
		vec4(-0.0f,+0.0f,1.0f,1.0f),//tengent
		vec3(-1.0f, -1.0f, -1.0f), // 14
		vec3(+0.4f, +0.7f, +0.7f), // Color
		vec3(-1.0f, +0.0f, -0.0f), // normal
		vec2(+0.0f, +0.0f),		   //uv
		vec4(-0.0f,+0.0f,1.0f,1.0f),//tengent
		vec3(-1.0f, -1.0f, +1.0f), // 15
		vec3(+0.2f, +0.5f, +1.0f), // Color
		vec3(-1.0f, +0.0f, -0.0f), // normal
		vec2(+1.0f, +0.0f),		   //uv
		vec4(-0.0f,+0.0f,1.0f,1.0f),//tengent

		vec3(+1.0f, +1.0f, +1.0f), // 16
		vec3(+0.6f, +1.0f, +0.7f), // Color
		vec3(-0.0f, +0.0f, +1.0f), // normal
		vec2(+1.0f, +1.0f),		   //uv
		vec4(+1.0f,+0.0f,0.0f,1.0f),//tengent
		vec3(-1.0f, +1.0f, +1.0f), // 17
		vec3(+0.6f, +0.4f, +0.8f), // Color
		vec3(-0.0f, +0.0f, +1.0f), // normal
		vec2(+0.0f, +1.0f),		   //uv
		vec4(+1.0f,+0.0f,0.0f,1.0f),//tengent
		vec3(-1.0f, -1.0f, +1.0f), // 18
		vec3(+0.2f, +0.8f, +0.7f), // Color
		vec3(-0.0f, +0.0f, +1.0f), // normal
		vec2(+0.0f, +0.0f),		   //uv
		vec4(+1.0f,+0.0f,0.0f,1.0f),//tengent
		vec3(+1.0f, -1.0f, +1.0f), // 19
		vec3(+0.2f, +0.7f, +1.0f), // Color
		vec3(-0.0f, +0.0f, +1.0f), // normal
		vec2(+1.0f, +0.0f),		   //uv
		vec4(+1.0f, +0.0f, 0.0f,1.0f),//tengent

		vec3(+1.0f, -1.0f, -1.0f), // 20
		vec3(+0.8f, +0.3f, +0.7f), // Color
		vec3(-0.0f, -1.0f, +0.0f), // normal
		vec2(+1.0f, +1.0f),		   //uv
		vec4(-0.0f, +0.0f, -1.0f, 1.0f),//tengent
		vec3(-1.0f, -1.0f, -1.0f), // 21
		vec3(+0.8f, +0.9f, +0.5f), // Color
		vec3(-0.0f, -1.0f, +0.0f), // normal
		vec2(+0.0f, +1.0f),		   //uv
		vec4(-0.0f, +0.0f, -1.0f, 1.0f),//tengent
		vec3(-1.0f, -1.0f, +1.0f), // 22
		vec3(+0.5f, +0.8f, +0.5f), // Color
		vec3(-0.0f, -1.0f, +0.0f), // normal
		vec2(+0.0f, +0.0f),		   //uv
		vec4(-0.0f, +0.0f, -1.0f, 1.0f),//tengent
		vec3(+1.0f, -1.0f, +1.0f), // 23
		vec3(+0.9f, +1.0f, +0.2f), // Color
		vec3(-0.0f, -1.0f, +0.0f), // normal
		vec2(+1.0f, +0.0f),		   //uv
		vec4(-0.0f, +0.0f, -1.0f, 1.0f),//tengent
	};

	data.numVertices = NUM_ELEMENT(CubeVerts);
	data.vertices = new Vertex[data.numVertices];
	memcpy(data.vertices, CubeVerts, sizeof(CubeVerts));

	unsigned short CubeIndices[] = {
		0,   1,  2,  0,  2,  3, // Top
		4,   5,  6,  4,  6,  7, // Front
		8,   9, 10,  8, 10, 11, // Right
		12, 13, 14, 12, 14, 15, // Left
		16, 17, 18, 16, 18, 19, // Back
		20, 22, 21, 20, 23, 22, // Bottom
	};
	data.numIndices = NUM_ELEMENT(CubeIndices);
	data.Indices = new GLushort[data.numIndices];
	memcpy(data.Indices, CubeIndices, sizeof(CubeIndices));

	return data;
}

Shapedata ShapeFactory::MakePlane(GLuint Divisions)
{
	Shapedata data;

	data.numVertices = Divisions * Divisions;
	data.vertices = new Vertex[data.numVertices];
	int half = Divisions / 2;
	for (int i = 0; i < Divisions; ++i)
	{
		for (int j = 0; j < Divisions; ++j)
		{
			Vertex& vert = data.vertices[i*Divisions + j];
			vert.position.x = j - half;
			vert.position.y = 0;
			vert.position.z = i - half;
			vert.color = randomColor();
			vert.normal = vec3(0.0f, 1.0f, 0.0f);
			vert.uv.x = ((float)j) / ((float)(Divisions-1));
			vert.uv.y = (1-((float)i) / ((float)(Divisions-1)));
			vert.tengent = vec4(1.0f, 0.0f, 0.0f, -1.0f);
		}
	}

	data.numIndices = (Divisions - 1) * (Divisions - 1) * 2 * 3;
	data.Indices = new GLushort[data.numIndices];

	int runner = 0;
	for (int row = 0; row < Divisions - 1; row++)
	{
		for (int col = 0; col < Divisions - 1; col++)
		{
			data.Indices[runner++] = Divisions * row + col;
			data.Indices[runner++] = Divisions * row + col + Divisions;
			data.Indices[runner++] = Divisions * row + col + Divisions + 1;

			data.Indices[runner++] = Divisions * row + col;
			data.Indices[runner++] = Divisions * row + col + Divisions + 1;
			data.Indices[runner++] = Divisions * row + col + 1;
		}
	}
	assert(runner = data.numIndices);
	return data;
}