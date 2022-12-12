#version 330 core

layout (location = 0) in vec3 vertexPosition;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


out vec3 fragPos;
out vec3 fNormal;

out vec3 vPos;

void main(){

    gl_Position = projection * view * model * vec4(vertexPosition, 1.0);
    fNormal = vertexPosition;
    fragPos = vec3(model * vec4(vertexPosition, 1.0));
    vPos = vec3(model * vec4(vertexPosition, 1.0));
    
}

