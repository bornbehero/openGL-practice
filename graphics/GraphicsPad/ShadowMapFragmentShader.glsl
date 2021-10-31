#version 430

in vec2 f_uv;

out vec4 pixelcolor;

uniform sampler2D MyTexture;

void main(){
	pixelcolor = texture(MyTexture,f_uv);
}
