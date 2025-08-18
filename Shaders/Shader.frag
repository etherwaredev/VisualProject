#version 460 core
out vec4 FragColor;

in VData {
	vec3 VPos;
} FVertData;

vec3 CenterPos = FVertData.VPos;
vec3 XPosNorm = CenterPos + vec3(0.0, 0.0, 0.0);
vec3 XNegNorm = CenterPos + vec3(0.0, 0.0, 0.0);
vec3 YPosNorm = CenterPos + vec3(0.0, 0.5, 0.0);
vec3 YNegNorm = CenterPos + vec3(0.0, 0.5, 0.0);
vec3 ZPosNorm = CenterPos + vec3(0.0, 0.0, 0.0);
vec3 ZNegNorm = CenterPos + vec3(0.0, 0.0, 0.0);

uniform vec3 LightPos;

uniform mat4 ProjMatrix, ViewMatrix;
mat4 MatrixMult = ProjMatrix * ViewMatrix;

void main() {
	vec4 DeepBlue = {.09f, .15f, .95f, 1.0f};

    FragColor = DeepBlue;
}
