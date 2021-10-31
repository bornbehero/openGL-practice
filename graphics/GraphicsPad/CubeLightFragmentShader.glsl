#version 430

in vec3 VertexColor;
out vec4 pixelcolor;

void main()
{
	pixelcolor = vec4(VertexColor,1.0);
}
