#version 430

in layout(location = 0) vec3 Position;
in layout(location = 1) vec3 Color;
in layout(location = 2) vec3 normal;
in layout(location = 3) vec2 uv;
in layout(location = 4) vec4 tengent;
out vec3 v2fcolor;
out vec3 AmbientLightColor;
out vec3 WorldPosition;
out vec2 f_uv;
out mat4 TengentToModelTransform;
out mat4 M2WMatrix;
out vec3 ObjectSpaceNormal;

uniform mat4 FullTransformMatrix;
uniform mat4 Model2WorldMatrix;
uniform vec3 AmbientLight;

void main()
{
	gl_Position = FullTransformMatrix * vec4(Position, 1.0);
	v2fcolor = Color;
	AmbientLightColor = AmbientLight;
	WorldPosition = vec3(Model2WorldMatrix * vec4(Position,1.0));
	f_uv = uv;
	vec3 BiotengentVector = normalize(cross(normal,vec3(tengent)) * tengent.w);
	TengentToModelTransform = mat4(vec4(vec3(tengent),0.0),vec4(BiotengentVector,0.0),vec4(normal,0.0),vec4(Position,1.0));
	M2WMatrix = Model2WorldMatrix;
	ObjectSpaceNormal = normal;
}
