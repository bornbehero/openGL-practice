#pragma once
#include <glm\glm.hpp>

struct Vertex 
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec4 tengent;
};
