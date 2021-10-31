#version 430

in vec3 Texcoord;

out vec4 pixelcolor;

uniform samplerCube CubeMap;

void main()
{
	pixelcolor = texture (CubeMap,Texcoord);
}
