const char* vertexshader =
"#version 430\r\n"
"in layout(location=0) vec2 Position;"
"in layout(location=1)  vec3 Color;"
"out vec3 v2fColor;"
"void main()"
"{"
"	gl_Position = vec4(Position,0,1);"
"	v2fColor = Color;"
"}";

const char* fragmentshader =
"#version 430\r\n"
"in vec3 v2fColor;"
"out vec4 OutColor;"
"void main()"
"{"
"OutColor = vec4(v2fColor,1);"
"}";
