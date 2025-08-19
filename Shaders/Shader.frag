#version 460 core
out vec4 FragColor;

//in vec4 gl_FragCoord;

in VData {
	vec3 VPos;
} FVertData;

vec3 XPosNorm = normalize(FVertData.VPos + vec3(0.5, 0.0, 0.0));
vec3 XNegNorm = normalize(FVertData.VPos + vec3(-0.5, 0.0, 0.0));
vec3 YPosNorm = normalize(FVertData.VPos + vec3(0.0, 0.5, 0.0));
vec3 YNegNorm = normalize(FVertData.VPos + vec3(0.0, -0.5, 0.0));
vec3 ZPosNorm = normalize(FVertData.VPos + vec3(0.0, 0.0, 0.5));
vec3 ZNegNorm = normalize(FVertData.VPos + vec3(0.0, 0.0, -0.5));

uniform vec3 LightPos;

uniform mat4 ProjMatrix, ViewMatrix;
mat4 MatrixMult = ProjMatrix * ViewMatrix;

void main() {
	vec4 ObjColor = {.09f, .15f, .95f, 1.0f};

	vec4 Ambient = (0.1 * vec4(1.0f, 1.0f, 1.0f, 1.0f)) * ObjColor;

	vec3 FCoord = normalize(vec3(gl_FragCoord.x, gl_FragCoord.y, gl_FragCoord.z));
	vec3 LightDirection = normalize(LightPos - YPosNorm);
	float Diffrence = max(dot(YPosNorm, LightDirection), 0.0);
	vec4 Diffuse = Diffrence * vec4(1.0f, 1.0f, 1.0f, 1.0f);

    FragColor = (Ambient + Diffuse) * ObjColor;
}
