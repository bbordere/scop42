#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 24) out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
const float voxelSize = 0.5;

void main() {
    vec3 position = gl_in[0].gl_Position.xyz - (voxelSize / 4);

    // Les sommets d'un cube unitaire
    vec3 vertices[8] = vec3[8](
        vec3(0.0, 0.0, 0.0),
        vec3(0.5, 0.0, 0.0),
        vec3(0.0, 0.5, 0.0),
        vec3(0.5, 0.5, 0.0),
        vec3(0.0, 0.0, 0.5),
        vec3(0.5, 0.0, 0.5),
        vec3(0.0, 0.5, 0.5),
        vec3(0.5, 0.5, 0.5)
    );

    vec3 cubeVertices[24] = vec3[24](
        vertices[0],
        vertices[1],
        vertices[2],
        vertices[3],

        vertices[4],
        vertices[5],
        vertices[6],
        vertices[7],

        vertices[0],
        vertices[1],
        vertices[4],
        vertices[5],

        vertices[2],
        vertices[3],
        vertices[6],
        vertices[7],

        vertices[0],
        vertices[2],
        vertices[4],
        vertices[6],

        vertices[1],
        vertices[3],
        vertices[5],
        vertices[7]
    );
    for (int i = 0; i < 24; ++i) {
        gl_Position = projection * view * model * vec4(position + cubeVertices[i] * voxelSize, 1.0);
        EmitVertex();
    }
    EndPrimitive();
}


// void build_house(vec4 position)
// {    
//     gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);    // 1:bottom-left
//     EmitVertex();
//     gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0);    // 2:bottom-right
//     EmitVertex();
//     gl_Position = position + vec4(-0.2,  0.2, 0.0, 0.0);    // 3:top-left
//     EmitVertex();
//     gl_Position = position + vec4( 0.2,  0.2, 0.0, 0.0);    // 4:top-right
//     EmitVertex();
//     gl_Position = position + vec4( 0.0,  0.4, 0.0, 0.0);    // 5:top
//     EmitVertex();
//     EndPrimitive();
// }

// void main() {    
//     build_house(gl_in[0].gl_Position);
// } 