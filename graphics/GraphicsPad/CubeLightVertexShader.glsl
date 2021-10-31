#version 430

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 Color;
in layout(location = 2) vec3 normal;

out vec3 VertexColor;

uniform mat4 LightTransformMatrix;

void main()
{
	gl_Position = LightTransformMatrix * vec4 (position,1.0);
	VertexColor = Color;
}
