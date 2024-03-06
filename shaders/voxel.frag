#version 330 core

out vec4 color;

float random (vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123);
}

void main() {
	color = vec4(1.0,0.5, 0.3, 0.5);
}
