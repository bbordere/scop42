#version 330 core

in vec2 TexCoords;

out vec4 fragColor;

uniform sampler2D depthMap;

void main(){
	float depth = texture(depthMap, TexCoords).r;
	fragColor = vec4(vec3(depth), 1.0);
}