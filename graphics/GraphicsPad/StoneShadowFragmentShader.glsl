#version 430


in vec3 AmbientLightColor;
in vec3 WorldPosition;
in vec2 f_uv;
in vec3 NormalWorld;
out vec4 pixelcolor;

uniform vec3 LightPosition;
uniform vec3 ViewPosition;
uniform sampler2D MyTexture;
uniform sampler2D NormalMap;
uniform float AttenuationFactor;
uniform sampler2D ShadowColorTex;
uniform sampler2D ShadowMap;
uniform mat4 LightFullTransformMatrix;

void main()
{
	//normalmap
	vec3 lightVector = normalize(LightPosition - WorldPosition);
	float distance = length(LightPosition - WorldPosition);
	float attenuation = 1 / (AttenuationFactor * pow(distance,2));
	vec3 WorldNormal = NormalWorld;

	vec4 texColor = texture(MyTexture,f_uv);
	
	
	vec4 LightclipPos = LightFullTransformMatrix * vec4(WorldPosition, 1.0);
	vec2 shadowmap_uv = vec2(LightclipPos.x / LightclipPos.w,LightclipPos.y / LightclipPos.w) ;
	vec2 shadowmap_uv_fixed = shadowmap_uv * 0.5 + 0.5;
	vec4 shadowmapColor = texture (ShadowMap,shadowmap_uv_fixed);
	float shadowed;
	vec3 ShadowColor = vec3(1,1,1);
	if(shadowmap_uv_fixed.x >= 1 || shadowmap_uv_fixed.x <=0 || shadowmap_uv_fixed.y >= 1 || shadowmap_uv_fixed.y <=0)
		shadowed = 1;
	else
	{
		shadowed = clamp((shadowmapColor.x - (LightclipPos.z / LightclipPos.w * 0.5 + 0.5) +0.01) * 100 ,0,1);
		ShadowColor = mix(texture(ShadowColorTex, shadowmap_uv_fixed).xyz, ShadowColor, shadowed);
	}
	float brightness = clamp(dot(lightVector, WorldNormal),0,1);
	vec4 diffuseLight =  vec4(ShadowColor,1) * attenuation * brightness * texColor;

	vec3 reflectionVec = reflect(-lightVector,WorldNormal);
	vec3 viewdirection = normalize(ViewPosition - WorldPosition);
	float specularIntensity = pow(dot(viewdirection, reflectionVec),50);
	vec3 specularlight = ShadowColor * specularIntensity * vec3(1.0,0.0,0.0);

	vec3 AmbientLight = AmbientLightColor * texColor.xyz;

//	pixelcolor = vec4(WorldNormal,1);
	pixelcolor = vec4(diffuseLight.xyz + AmbientLight , 1.0);
//	pixelcolor = vec4(clamp(vec3(specularIntensity,specularIntensity,specularIntensity),0,1) + AmbientLightColor,1.0);
	
}
