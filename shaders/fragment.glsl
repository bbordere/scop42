#version 330 core

in vec3 color;
in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 lightAmbientIntensity;
uniform vec3 lightDiffuseIntensity;
uniform vec3 lightSpecularIntensity;
uniform vec3 camPos;

uniform float factor;
uniform sampler2D ourTexture;

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
	return (color * (ambient + diffuse + specular));
}


void main() {
	vec3 pixelColor = color;
	FragColor = getLight(mix(texture(ourTexture, texCoord), vec4(color, 1.0), factor));
}