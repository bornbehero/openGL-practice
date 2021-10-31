#version 430

in layout(location = 0) vec3 Position;
in layout(location = 1) vec3 Color;
in layout(location = 2) vec3 normal;
in layout(location = 3) vec2 uv;
in layout(location = 4) vec4 tengent;

out vec2 f_uv;

uniform mat4 FullTransformMatrix;

void main(){
	gl_Position = FullTransformMatrix * vec4(Position,1.0f);
	f_uv = uv;
}
