#version 330 core

// Input vertex data, different for all executions of this shader.
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 texPosition;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;




out vec2 texPos;

void main(){


    gl_Position = projection * view * model * vec4(vertexPosition, 1.0);

    texPos = texPosition;
}

