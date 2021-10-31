#version 430

in vec3 AmbientLightColor;
in vec3 WorldPosition;
in vec2 f_uv;
in vec3 NormalWorld;
in mat4 TangentToWorldTransform;
out vec4 pixelcolor;

uniform vec3 LightPosition;
uniform vec3 ViewPosition;
uniform sampler2D MyTexture;
uniform float AttenuationFactor;
uniform sampler2D RoughnessTexture;
uniform sampler2D MetallicTexture;
uniform sampler2D AlphaTexture;
uniform sampler2D NormalMap;

void main()
{
	vec3 lightVector = normalize(LightPosition - WorldPosition);
	float distance = length(LightPosition - WorldPosition);
	float attenuation = 1 / (AttenuationFactor * pow(distance,2));
	//NormalMap
	vec4 normalmapcolor = texture(NormalMap,f_uv);
	vec3 FixedNormalVector = (normalmapcolor.xyz - vec3(0.5f,0.5f,0.5f))*2;
	vec3 WorldNormal = vec3(TangentToWorldTransform * vec4(FixedNormalVector,0.0));
	//vec3 WorldNormal = NormalWorld;

	vec4 texColor = texture(MyTexture,f_uv);
	vec4 AlphaValue = texture(AlphaTexture,f_uv);

	float brightness = clamp(dot(lightVector, WorldNormal),0,1);
	vec4 diffuseLight =  attenuation * brightness * texColor;

	vec3 reflectionVec = reflect(-lightVector,WorldNormal);
	vec3 viewdirection = normalize(ViewPosition - WorldPosition);
	float specularIntensity = pow(dot(viewdirection, reflectionVec),50);
	vec3 specularlight = specularIntensity * vec3(1.0,1.0,1.0);

	vec3 AmbientLight = AmbientLightColor * texColor.xyz;

//	pixelcolor = vec4(WorldNormal,1);
	pixelcolor = vec4(diffuseLight.xyz + AmbientLight + clamp(specularlight,0.0,1.0), AlphaValue.x);
//	pixelcolor = vec4(clamp(vec3(specularIntensity,specularIntensity,specularIntensity),0,1) + AmbientLightColor,1.0);
	
}
