#version 430

in layout(location = 0) vec3 Position;
in layout(location = 1) vec3 normal;
in layout(location = 2) vec3 uv;


out vec3 AmbientLightColor;
out vec3 WorldPosition;
out vec2 f_uv;
out vec3 NormalWorld;
out mat4 TangentToWorldTransform;

uniform mat4 FullTransformMatrix;
uniform mat4 Model2WorldMatrix;
uniform vec3 AmbientLight;

void main()
{
	gl_Position = FullTransformMatrix * vec4(Position, 1.0);
	AmbientLightColor = AmbientLight;
	WorldPosition = vec3(Model2WorldMatrix * vec4(Position,1.0));
	f_uv = vec2(uv);

	NormalWorld = vec3 (transpose(inverse(Model2WorldMatrix)) * vec4(normal,0.0));
	vec3 tangentModel = vec3(1,0,0);
	vec3 tangentWorld = vec3(Model2WorldMatrix * vec4(tangentModel,0.0));
	vec3 biNormalWorld = normalize(cross(NormalWorld,vec3(tangentWorld)));
	TangentToWorldTransform = mat4(vec4(tangentWorld,0.0),vec4(biNormalWorld,0.0),vec4(NormalWorld,0.0),vec4(WorldPosition,1.0));
}