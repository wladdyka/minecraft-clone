#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texture;
layout (location = 2) in vec3 norm;

out vec4 vertexColour;
out vec2 textureCoords;
out vec3 normal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main() {
    gl_Position = projection * view * model * vec4(pos, 1.0);
    vertexColour = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);

    textureCoords = texture;

    normal = mat3(transpose(inverse(model))) * norm;
    fragPos = (model * vec4(pos, 1.0)).xyz;
}