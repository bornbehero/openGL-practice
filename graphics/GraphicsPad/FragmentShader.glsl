#version 430

in vec3 v2fcolor;
in vec3 AmbientLightColor;
in vec3 WorldPosition;
in vec2 f_uv;
in mat4 TengentToModelTransform;
in mat4 M2WMatrix;
in vec3 ObjectSpaceNormal;
out vec4 pixelcolor;

uniform vec3 LightPosition;
uniform vec3 ViewPosition;
uniform sampler2D MyTexture;
uniform sampler2D NormalMap;
uniform float AttenuationFactor;

void main()
{
	//normalmap

	vec4 normalmapcolor = texture(NormalMap,f_uv);
	vec3 FixedNormalVector = (normalmapcolor.xyz - vec3(0.5f,0.5f,0.5f))*2;
	vec3 ModelNormal = vec3(TengentToModelTransform * vec4(FixedNormalVector,0.0));

	vec3 lightVector = normalize(LightPosition - WorldPosition);
	float distance = length(LightPosition - WorldPosition);
	float attenuation = 1 / (AttenuationFactor * pow(distance,2));
	vec3 PreWorldNormal = vec3 (transpose(inverse(M2WMatrix)) * vec4(ObjectSpaceNormal,0));
	vec3 ActualModelNormal = mix(ObjectSpaceNormal,ModelNormal,clamp(dot(lightVector, PreWorldNormal)*10000,0,1));
	vec3 WorldNormal = vec3 (transpose(inverse(M2WMatrix)) * vec4(ActualModelNormal,0));

	vec4 texColor = texture(MyTexture,f_uv);

	
	float brightness = clamp(dot(lightVector, WorldNormal),0,1);
	vec4 diffuseLight =  attenuation * brightness * texColor;

	vec3 reflectionVec = reflect(-lightVector,WorldNormal);
	vec3 viewdirection = normalize(ViewPosition - WorldPosition);
	float specularIntensity = pow(dot(viewdirection, reflectionVec),50);
	vec3 specularlight = specularIntensity * vec3(1.0,1.0,1.0);

	vec3 AmbientLight = AmbientLightColor * texColor.xyz;

//	pixelcolor = vec4(WorldNormal,1);
	pixelcolor = vec4(diffuseLight.xyz + AmbientLight + clamp(specularlight,0.0,1.0), 1.0);
//	pixelcolor = vec4(clamp(vec3(specularIntensity,specularIntensity,specularIntensity),0,1) + AmbientLightColor,1.0);
	
}
