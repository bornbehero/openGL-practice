#version 430

in layout(location = 0) vec3 Position;
in layout(location = 1) vec3 Color;
in layout(location = 2) vec3 normal;
in layout(location = 3) vec2 uv;
in layout(location = 4) vec4 tengent;

out vec3 Texcoord;

uniform mat4 SkyboxTransformMatrix;

void main()
{
	gl_Position = SkyboxTransformMatrix * vec4(Position,1.0);
	Texcoord = -Position;
}
