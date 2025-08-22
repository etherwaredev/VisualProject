#version 460 core
out vec4 FragColor;

uniform mat4 ProjMatrix, ViewMatrix;
mat4 MatrixMult = ProjMatrix * ViewMatrix;

void main() {
    FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}