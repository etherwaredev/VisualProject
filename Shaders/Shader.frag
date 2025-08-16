#version 460 core
out vec4 FragColor;

uniform mat4 ProjMatrix, ViewMatrix;
mat4 MatrixMult = ProjMatrix * ViewMatrix;

void main() {
	vec4 DeepBlue = {.09f, .15f, .95f, 1.0f};
	vec4 Pinkish = {.94f, 0.3f, .85f, 1.0f};

    FragColor = mix(Pinkish, DeepBlue, normalize(gl_FragCoord * MatrixMult));
}
