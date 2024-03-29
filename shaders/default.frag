#version 330 core

in vec3 color;
in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;
in vec4 fragPosLight;
in vec3 vPos;
in vec3 vNormal;

out vec4 fragColor;

uniform vec3 lightPos;
uniform vec3 lightAmbientIntensity;
uniform vec3 lightDiffuseIntensity;
uniform vec3 lightSpecularIntensity;
uniform vec3 camPos;
uniform float factor;
uniform sampler2D textureSampler;
uniform sampler2D shadowMap;
uniform int uvMappingMode;
uniform float scale;

float shadowCalc(float dotLight){
	vec3 pos = fragPosLight.xyz / fragPosLight.w;
	pos = pos * 0.5 + 0.5;
	if (pos.z > 1.0){
		pos.z = 1.0;
	}
	// float bias = max(0.05 * (1.0 - dotLight), 0.0005);
	// float bias = 0.0005 * tan(acos(dotLight));
	// bias = clamp(bias, 0, 0.005);
	float bias = 0.0000;

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for (int x = -1; x <= 1; ++x){
		for (int y = -1; y <= 1; ++y){
			float depth = texture(shadowMap, pos.xy + vec2(x, y) * texelSize).r;
			shadow += (depth + bias) < pos.z ? 0.0 : 1.0;
		}
	}
	return (shadow / 9.0);
}

vec4 getLight(vec4 color){
	vec3 Normal = normalize(normal);

	vec3 ligthDir = normalize(lightPos - fragPos);
	float diff = max(0, dot(ligthDir, Normal));
	vec4 diffuse = vec4(diff * lightDiffuseIntensity, 1.0);

	vec3 viewDir = normalize(camPos - fragPos);
	vec3 reflectDir = reflect(-ligthDir, Normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0), 32);
	vec4 specular = vec4(spec * lightSpecularIntensity, 1.0);

	vec4 ambient = vec4(lightAmbientIntensity, 1.0);
	return (color * (ambient + (shadowCalc(dot(ligthDir, Normal)) * (diffuse + specular))));
}

#define	PI 3.14159265358979323846

vec2 sphericalUv(vec3 pos)
{
	pos = normalize(pos * 0.5);
	float scale = PI / 6;
	return vec2(0.5 + atan(pos.z, pos.x) / (2.0 * scale), 0.5 + asin(pos.y) / scale);
}

vec4 getTextureColor(){
	if (uvMappingMode == 0)
		return (texture(textureSampler, texCoord * scale));
	return (texture(textureSampler, sphericalUv(vPos)));
}

void main() {
	fragColor = getLight(mix(getTextureColor(), vec4(color, 1.0), factor));
}