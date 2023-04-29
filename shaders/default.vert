#version 330 core

layout (location=0) in vec3 position;
layout (location=1) in vec3 normal;
layout (location=2) in vec3 color;
layout (location=3) in vec2 texCoord;

out vec3 vertexColor;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 mvp;
uniform mat4 model;

void main() {
    gl_Position = mvp * vec4(position, 1.0);
    vertexColor = color;
    TexCoord = texCoord;
    Normal = normal;
    FragPos = vec3(model * vec4(position, 1.0));
}