#version 330 core

// Input vertex data, different for all executions of this shader.
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 normal;

uniform mat4 view;
uniform mat4 projection;


out vec3 fragPos;
out vec3 fNormal;

void main(){


    gl_Position = projection * view * vec4(vertexPosition, 1.0);
    fNormal = normal;
    fragPos = vertexPosition;
}

