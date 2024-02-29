#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;

out vec3 color;
out vec2 texCoord;
out vec3 normal;
out vec3 fragPos;
out vec4 fragPosLight;
out vec3 vPos;
out vec3 vNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main() {
	normal = mat3(transpose(inverse(model))) * normalize(aNormal);
	fragPos = vec3(model * vec4(aPos, 1.0));
	color = aColor;
	texCoord = aTexCoord;
	fragPosLight = lightSpaceMatrix * vec4(fragPos, 1.0);
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	vPos = aPos;
}